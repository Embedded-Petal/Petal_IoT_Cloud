#ifndef PETAL_CONFIG_H
#define PETAL_CONFIG_H

/************* SERVER CONFIG *************/
#define PETAL_SERVER_IP   "http://api.iotcloud.petalred.com" //"192.168.1.30:8080"//
#define PETAL_SERVER_PORT 80

/************* BOARD AUTO DETECTION *************/
#if defined(ESP32)
  #define PETAL_BOARD_ESP32
#elif defined(ESP8266)
  #define PETAL_BOARD_ESP8266
#else
  #warning "Petal supports only ESP32 and ESP8266"
#endif

/************* COMMON LIBS *************/
#include <Arduino.h>
#include <ArduinoJson.h>

/************* BOARD SPECIFIC LIBS *************/
#ifdef PETAL_BOARD_ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#endif

#ifdef PETAL_BOARD_ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

#endif
