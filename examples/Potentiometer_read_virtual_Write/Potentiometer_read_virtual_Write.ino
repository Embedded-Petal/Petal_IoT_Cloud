#include <Petal_IoT_Cloud.h>

#define WIFI_SSID "*******"
#define WIFI_PASSWORD "**********"
#define DEVICE_TOKEN "***********"

#define Potentiometer 34

unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);
  pinMode(Potentiometer, INPUT);

  Cloud.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
}

void loop() {
  Cloud.loop();
  int a = analogRead(Potentiometer);
  if (millis() - previousMillis > 2000) //2 second once data will send
  {
    Cloud.write("V4", a);
    Serial.println(a);
    previousMillis = millis();
  }
}
