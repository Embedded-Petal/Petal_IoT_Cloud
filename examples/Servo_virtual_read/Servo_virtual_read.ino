#include <Petal_IoT_Cloud.h>
#include <ESP32Servo.h>

#define WIFI_SSID  "*******"
#define WIFI_PASSWORD "**********"
#define DEVICE_TOKEN "***********"

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
