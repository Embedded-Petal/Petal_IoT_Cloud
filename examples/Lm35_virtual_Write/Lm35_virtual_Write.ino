#include "iotCloud.h"

#define WIFI_SSID "*******"
#define WIFI_PASSWORD "**********"
#define DEVICE_TOKEN "***********"

#define ADC_VREF_mV 3300.0  // in millivolt
#define ADC_RESOLUTION 4096.0
#define PIN_LM35 36  // ESP32 pin GPIO36 (ADC0) connected to LM35
float lastTempFValue = 0, lastTempCValue = 0;
void lm35() {
  // read the ADC value from the temperature sensor
  int adcVal = analogRead(PIN_LM35);
  // convert the ADC value to voltage in millivolt
  float milliVolt = adcVal * (ADC_VREF_mV / ADC_RESOLUTION);
  // convert the voltage to the temperature in °C
  float tempC = milliVolt / 10;
  // convert the °C to °F
  float tempF = tempC * 9 / 5 + 32;

  //Print the Temperature in IoT Cloud

  Cloud.write("V8", tempC);
  Serial.print("Temperature: ");
  Serial.print(tempC);  // print the temperature in °C
  Serial.print("°C");

  Serial.println(" ~ ");

  //Print the fahrenheit in IoT Cloud
  Cloud.write("V5", tempF);
  Serial.print(tempF);  // print the temperature in °F
  Serial.println("°F");
  delay(500);
}


void setup() {
  Serial.begin(115200);
  // set the ADC attenuation to 11 dB (up to ~3.3V input)
  analogSetAttenuation(ADC_11db);
  Cloud.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
}

void loop() {
  Cloud.loop();
  lm35();
}
