#include <iotCloud.h>

#define WIFI_SSID  "*******"
#define WIFI_PASSWORD "**********"
#define DEVICE_TOKEN "***********"

#define MOTOR_PIN1 13  // Motor driver IN1
#define MOTOR_PIN2 12  // Motor driver IN2
#define MOTOR_PIN3 14  // Motor driver Enable pin (PWM)
#define MOTOR_PIN4 27  // Motor driver Enable pin (PWM)

void setup() {
  // Initialize the motor driver pins
  pinMode(MOTOR_PIN1, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);
  pinMode(MOTOR_PIN3, OUTPUT);
  pinMode(MOTOR_PIN4, OUTPUT);
  Serial.begin(115200);
  Cloud.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
  Cloud.registerPin("V16", [](String V) {
    int motorSpeed = V.toInt();  
    // Set motor direction and speed
    analogWrite(MOTOR_PIN1, motorSpeed);
    analogWrite(MOTOR_PIN2, 0);
    analogWrite(MOTOR_PIN3, 0);
    analogWrite(MOTOR_PIN4, motorSpeed);
    delay(100);  // Small delay for stability
  });
}

void loop() {
  Cloud.loop();
}