#include <Petal_IoT_Cloud.h>

#define WIFI_SSID "*******"
#define WIFI_PASSWORD "**********"
#define DEVICE_TOKEN "***********"

#define Potentiometer 34


void setup() {
  Serial.begin(115200);
  pinMode(Potentiometer, INPUT);
 
  Cloud.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
}

void loop() {
  Cloud.loop();
  int a = analogRead(Potentiometer);
  Cloud.write("V4", a);
  Serial.println(a);
  delay(100);
 
}
