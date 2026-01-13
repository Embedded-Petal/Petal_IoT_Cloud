#  Petal Arduino Library

Petal is a universal IoT HTTP communication library for ESP32 and ESP8266.

## Features
- ESP32 & ESP8266 support
- Virtual pin read & write
- RGB HEX color parsing
- Common polling interval
- Automatic board detection

## Supported Boards
- ESP32
- ESP8266

## Installation
1. Open Arduino IDE
2. Go to Library Manager
3. Search for "IoTCloud" , "ArduinoJson"
4. Install

## Example
```
#include <iotCloud.h>
#define WIFI_SSID     "SSID"
#define WIFI_PASSWORD "Password"
#define DEVICE_TOKEN  "Auth token"

#define LED1 13
int LEDState1;

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW);
  Cloud.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
  Cloud.registerPin("V0", [](String v) {
    LEDState1 = v.toInt();
  });
}

void loop() {
  Cloud.loop();
  if (LEDState1 == 1 ) digitalWrite(LED1, HIGH);
  else digitalWrite(LED1, LOW);
}  

```
