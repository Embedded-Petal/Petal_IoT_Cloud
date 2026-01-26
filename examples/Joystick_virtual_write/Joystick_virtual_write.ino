#include "iotCloud.h"

#define WIFI_SSID "Petal"
#define WIFI_PASSWORD "Petal#2024"
#define DEVICE_TOKEN "dc971248626d4ef087646621637c4f9d1769065895799"

// Define the joystick pins
#define JOYSTICK_X_PIN 4
#define JOYSTICK_Y_PIN 15

String vPinX = "V1";
String vPinY = "V2";
void setup() {
  // Initialize the serial communication
  Serial.begin(115200);

  // Set joystick pins as input
  pinMode(JOYSTICK_X_PIN, INPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);
  Cloud.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
  Serial.println("Dual-Axis Joystick Data Monitoring");
}

void loop() {
  Cloud.loop();
  // Read analog values from the joystick
  int xValue = analogRead(JOYSTICK_X_PIN);
  int yValue = analogRead(JOYSTICK_Y_PIN);

  // Map the values to 8-bit ADC (0-255)
  int xMapped = map(xValue, 0, 4095, 0, 255);
  int yMapped = map(yValue, 0, 4095, 0, 255);

  // Print the mapped values
  Serial.print("X-Axis : ");
  Serial.println(xMapped);
  Serial.print("Y-Axis : ");
  Serial.println(yMapped);
  Cloud.write(vPinX, xMapped);
  Cloud.write(vPinY, yMapped);
  delay(300); // Delay for stability
}