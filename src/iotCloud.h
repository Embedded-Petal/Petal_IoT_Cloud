#ifndef IOTCLOUD_H
#define IOTCLOUD_H

#include "Petal_Config.h"
#include <Arduino.h>
#include <WiFi.h>
#include "WebSocketsClient.h"
#include <ArduinoJson.h>
#include <map>
#include <functional>
#include <Update.h>
#include <Preferences.h>

typedef std::function<void(String)> PinCallback;

class IoTCloud {

  public:
    struct RGB {
      uint8_t r;
      uint8_t g;
      uint8_t b;
    };

    WebSocketsClient ws;
    std::map<String, PinCallback> callbacks;
    std::map<String, String> lastValues;

    template<typename T>
    bool write(String pin, T value) {
        return writeInternal(pin, String(value));
    }

    String ssid, password, deviceToken;

    void begin(String ssid, String password, String token);
    void loop();

    // READ
    String read(String pin);
    int readInt(String pin);
    bool readBool(String pin);
    RGB readRGB(String pin);
    bool writeAck(String pin, String value);
    // CALLBACK
    void registerPin(String pin, PinCallback cb);
    void writeRawWS(String pin, String value);
    void storeMemoryString(String keyss, String values);
    void storeMemoryInt(String keyss, int values);

  private:
   bool writeInternal(String pin, String value);
    void dispatchPin(String pin, String value);
    void sendSTOMP(String f);
    void connectWiFi();
    void connectWS();
    int pinIndex(String pin);

    bool isHexColor(String v);
    uint8_t hexByte(String h);

    static void wsEvent(WStype_t type, uint8_t *payload, size_t length);

    int _lastWriteValue[500];
    String _lastWriteString[500];
    RGB _lastRGB[500];
    String urlEncode(const String &value);
    void updates(String url);
};

extern IoTCloud Cloud;
extern Preferences preferences;

#endif

