#include "iotcloud.h"

String WS_HOST = "api.iotcloud.petalred.com";
uint16_t WS_PORT = 443;
String WS_PATH = "/ws-mobile";

IoTCloud Cloud;
Preferences preferences;
static IoTCloud *instancePtr;
String key = "/api/iot/mcu?token=";
unsigned long lastStatusSend = 0;
static uint32_t lastWs = 0;
int getIntMemory;
String airValues = "";

void IoTCloud::storeMemoryString(String keyss, String values) {
  preferences.begin("cloud", false);  //read&write
  preferences.putString(keyss.c_str(), values.c_str());
  preferences.end();
}

void IoTCloud::storeMemoryInt(String keyss, int values) {
  preferences.begin("cloud", false);
  preferences.putInt(keyss.c_str(), values);
  preferences.end();
}

void IoTCloud::begin(String ssid, String password, String token) {
  preferences.begin("cloud", false);
  getIntMemory = preferences.getInt("update", 0);
  preferences.end();
  
  instancePtr = this;
  this->ssid = ssid;
  this->password = password;
  this->deviceToken = token;

  for (int i = 0; i < 500; i++) {
    _lastWriteValue[i] = -1;
    _lastRGB[i] = { 0, 0, 0 };
    _lastWriteString[i] = "";
  }

  connectWiFi();
  connectWS();
  uint32_t t0 = millis();
  while (millis() - t0 < 1000) {  // 1 second handshake window
    ws.loop();
    delay(1);
  }
}

void IoTCloud::connectWiFi() {
  WiFi.begin(ssid.c_str(), password.c_str());
  unsigned long t = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - t < 10000) {
    delay(200);
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi Connected..");
    if (!ws.isConnected()) {
      connectWS();
    }
  }
}



void IoTCloud::connectWS() {
  ws.beginSSL(WS_HOST.c_str(), WS_PORT, WS_PATH.c_str());
  // Heartbeat (important for cloud)
  ws.enableHeartbeat(15000, 8000, 2);
  // STOMP event handler
  ws.onEvent(IoTCloud::wsEvent);
}


void IoTCloud::registerPin(String pin, PinCallback cb) {
  callbacks[pin] = cb;
}
void IoTCloud::loop() {
  if (WiFi.status() != WL_CONNECTED)
    connectWiFi();

  // Auto reconnect WS
  if (!ws.isConnected() && WiFi.status() == WL_CONNECTED) {
    static unsigned long lastTry = 0;
    if (millis() - lastTry > 3000) {
      ws.disconnect();
      connectWS();
      lastTry = millis();
    }
  }
  else if (ws.isConnected() && WiFi.status() == WL_CONNECTED) {
    if (millis() - lastStatusSend >= 10000) {  // 10 seconds
      writeAck("status", "Online");
      if (getIntMemory == 1) {
        preferences.begin("cloud", false);
        String getStringMemory = preferences.getString("airStatus", "");
        for (int i = 0; i < 2; i++) {
          writeAck("airStatus", getStringMemory);
          delay(500);
        }
        preferences.putInt("update", 0);
        preferences.end();
        getIntMemory = 0;
      }
      lastStatusSend = millis();
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    if (millis() - lastWs >= 20) {  // 50Hz WebSocket
      ws.loop();
      lastWs = millis();
    }
  }
  yield();  // or delay(0)
}

/**************** DISPATCH ****************/

void IoTCloud::dispatchPin(String pin, String value) {
  lastValues[pin] = value;

  if (isHexColor(value)) {
    int idx = pinIndex(pin);
    String val = value;
    if (val.startsWith("#")) val.remove(0, 1);

    _lastRGB[idx].r = hexByte(val.substring(0, 2));
    _lastRGB[idx].g = hexByte(val.substring(2, 4));
    _lastRGB[idx].b = hexByte(val.substring(4, 6));
  }

  if (callbacks.count(pin)) callbacks[pin](value);
}

/**************** READ ****************/

String IoTCloud::read(String pin) {
  return lastValues[pin];
}

int IoTCloud::readInt(String pin) {
  return lastValues[pin].toInt();
}

bool IoTCloud::readBool(String pin) {
  return lastValues[pin] == "1";
}

IoTCloud::RGB IoTCloud::readRGB(String pin) {
  int idx = pinIndex(pin);
  if (idx < 0 || idx >= 500) return { 0, 0, 0 };
  return _lastRGB[idx];
}

/**************** WRITE (Unified) *************/

String IoTCloud::urlEncode(const String &value) {
  String encoded = "";
  char c;
  char bufHex[4];
  for (int i = 0; i < value.length(); i++) {
    c = value.charAt(i);
    if (isalnum(c)) {
      encoded += c;
    } else if (c == ' ') {
      encoded += "%20";
    } else {
      sprintf(bufHex, "%%%02X", c);
      encoded += bufHex;
    }
  }
  return encoded;
}

bool IoTCloud::writeAck(String pin, String value) {
  if (!ws.isConnected()) return false;
  int idx = pinIndex(pin);
  String encodedValue = urlEncode(value);
  String msg =
    "SEND\n"
    "destination:/app/device/"
    + instancePtr->deviceToken + "\n\n" + pin + "=" + encodedValue + '\0';
  ws.sendTXT(msg);
  return true;
}


bool IoTCloud::writeInternal(String pin, String value) {
 
  int idx = pinIndex(pin);
  String encodedValue = urlEncode(value);

  if (WiFi.status() != WL_CONNECTED) return false;
  
  String url = String(PETAL_SERVER_IP) + key + deviceToken + "&" + pin + "=" + encodedValue;
  HTTPClient https;
  https.begin(url);
  https.setTimeout(8000);
  int code = https.GET();
  https.end();

  if (code > 0) {
    _lastWriteString[idx] = encodedValue;  // cache string
    return true;
  }
  return false;
}

/**************** HELPERS ****************/

int IoTCloud::pinIndex(String pin) {
  pin.replace("V", "");
  return pin.toInt();
}

bool IoTCloud::isHexColor(String v) {
  if (v.startsWith("#")) v = v.substring(1);
  if (v.length() != 6) return false;
  for (int i = 0; i < 6; i++)
    if (!isxdigit(v[i])) return false;
  return true;
}

uint8_t IoTCloud::hexByte(String h) {
  return strtol(h.c_str(), NULL, 16);
}



/*********Check For Updates********** */
void IoTCloud::updates(String url) {
  HTTPClient http;
  http.begin(url);            // Specify the URL
  int httpCode = http.GET();  // Make the request

  if (httpCode == HTTP_CODE_OK) {  // Check for the returning code
    int contentLength = http.getSize();
    bool canBegin = Update.begin(contentLength);
    if (canBegin) {
      Serial.println("Begin OTA update...");

      WiFiClient *client = http.getStreamPtr();
      size_t written = Update.writeStream(*client);

      if (written == contentLength) {
        Serial.println("Written : " + String(written) + " successfully");
      } else {
        Serial.println("Written only : " + String(written) + "/" + String(contentLength));
      }

      if (Update.end()) {
        Serial.println("OTA done!");
        if (Update.isFinished()) {
          Serial.println("Update successfully completed. Rebooting.");
          airValues = "Success";
          storeMemoryString("airStatus", airValues);
          storeMemoryInt("update", 1);
          delay(1000);
          ESP.restart();
        } else {
          airValues = "Update not finished? Something went wrong!";
          writeAck("airStatus", airValues);

          Serial.println("Update not finished? Something went wrong!");
          delay(1000);
        }
      } else {
        airValues = "Error Occurred. Error #: " + String(Update.getError());
        writeAck("airStatus", airValues);
        Serial.println("Error Occurred. Error #: " + String(Update.getError()));
        delay(1000);
      }
    } else {
      airValues = "Not enough space to begin OTA";
      writeAck("airStatus", airValues);
      Serial.println("Not enough space to begin OTA");
      delay(1000);
    }
  } else {
    airValues = "Firmware not found on server, Error: " + String(httpCode);
    writeAck("airStatus", airValues);
    Serial.println("Firmware not found on server, Error: " + String(httpCode));
    delay(1000);
  }

  http.end();  // Close connection
}

/**************** STOMP WS ****************/

void IoTCloud::sendSTOMP(String f) {
  f += "\n\n";
  f += '\0';
  ws.sendTXT(f);
}


void IoTCloud::wsEvent(WStype_t type, uint8_t *payload, size_t length) {
  String msg = (char *)payload;
  switch (type) {
    case WStype_CONNECTED:
      {
        Serial.println("Petal Cloud CONNECT");
        instancePtr->sendSTOMP(
          "CONNECT\n"
          "accept-version:1.2\n"
          "host:api.iotcloud.petalred.com\n"
          "heart-beat:10000,10000");
        break;
      }

    case WStype_TEXT:
      {
        // CONNECTED frame
        if (msg.startsWith("CONNECTED")) {
          instancePtr->sendSTOMP(
            "SUBSCRIBE\n"
            "id:sub-0\n"
            "ack:auto\n"
            "destination:/topic/device/"
            + instancePtr->deviceToken);
          break;
        }

        // MESSAGE frame
        if (msg.startsWith("MESSAGE")) {
          int idx = msg.indexOf("\n\n");
          if (idx > 0) {
            String body = msg.substring(idx + 2);
            size_t capacity = body.length() * 1.2 + 256;
            DynamicJsonDocument doc(capacity);
            if (deserializeJson(doc, body) == DeserializationError::Ok) {
              String pin = doc["pin"].as<String>();
              String val = doc["value"].as<String>();
              val.trim();
              pin.trim();
              pin.toUpperCase();
              Serial.println(msg);
              if (pin == "AIR") {
                Serial.println("Air Update!");
                instancePtr->updates(val);
              } else {
                instancePtr->dispatchPin(pin, val);
              }
            }
          }
        }
        break;
      }    
  }
}



