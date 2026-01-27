#include <Petal_IoT_Cloud.h>

#define WIFI_SSID "*******"
#define WIFI_PASSWORD "**********"
#define DEVICE_TOKEN "***********""

#define LDR 33


void setup() {
  Serial.begin(115200);
  pinMode(LDR, INPUT);
 
  Cloud.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
}

void loop() {
  Cloud.loop();
  float a = analogRead(LDR);
  int percentage = (a / 4095) * 100 ;
  Cloud.write("V5", percentage);
  Serial.println(percentage);
  delay(100);
 
}
