#ifndef IOTCLOUD_H
#define IOTCLOUD_H

#include "Petal_Config.h"
#include <Arduino.h>
#include <WiFi.h>
//#include "PetaliotCloudws.h"
#include "WebSocketsClient.h"
#include <ArduinoJson.h>
#include <map>
#include <functional>

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

    String ssid, password, deviceToken;

    void begin(String ssid, String password, String token);
    void loop();

    // READ
    String read(String pin);
    int readInt(String pin);
    bool readBool(String pin);
    RGB readRGB(String pin);

    // WRITE
    bool write(String pin, int value);
    bool write(String pin, String value);

    // CALLBACK
    void registerPin(String pin, PinCallback cb);

  private:
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
};

extern IoTCloud Cloud;

#endif
