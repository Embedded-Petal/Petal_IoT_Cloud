#include <iotCloud.h>
#include <ESP32Servo.h>

#define WIFI_SSID     "Petal"
#define WIFI_PASSWORD  "Petal#2024"
#define DEVICE_TOKEN  "dc971248626d4ef087646621637c4f9d1769065895799"

#define Servo_pin 32
String vPinRead = "V1";
Servo servo;
int degree = 0;

void setup() {
  Serial.begin(115200);
  servo.attach(Servo_pin);
  Cloud.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
  Cloud.registerPin(vPinRead, [](String v) {
    degree = v.toInt();
    servo.write(degree);
    Serial.println("Servo Angle - " + String(degree));
  });
}

void loop() {
  Cloud.loop();
  
}  
