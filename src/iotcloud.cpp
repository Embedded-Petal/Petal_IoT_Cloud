#include "iotcloud.h"

String WS_HOST = "api.iotcloud.petalred.com";
uint16_t WS_PORT = 80;
String WS_PATH = "/ws-mobile";

IoTCloud Cloud;
static IoTCloud *instancePtr;
String key = "/api/iot/mcu?token=";

void IoTCloud::begin(String ssid, String password, String token) {
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
}

void IoTCloud::connectWiFi() {
  WiFi.begin(ssid.c_str(), password.c_str());
  unsigned long t = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - t < 10000) {
    delay(200);
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi Connected..");
  }
}



void IoTCloud::connectWS() {
  ws.begin(WS_HOST.c_str(), WS_PORT, WS_PATH.c_str());
  // Heartbeat (important for cloud)
  ws.enableHeartbeat(15000, 3000, 2);
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

  ws.loop();
}

/**************** DISPATCH ****************/

void IoTCloud::dispatchPin(String pin, String value) {
  if (lastValues[pin] == value) return;
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


bool IoTCloud::writeInternal(String pin, String value) {
 
  int idx = pinIndex(pin);
  String encodedValue = urlEncode(value);
  // avoid re-send if same string exists
  if (_lastWriteString[idx] == encodedValue) return true;

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
              instancePtr->dispatchPin(pin, val);
            }
          }
        }
        break;
      }       
  }
}
