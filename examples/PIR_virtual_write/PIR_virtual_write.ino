#include <Petal_IoT_Cloud.h>

#define WIFI_SSID "*******"
#define WIFI_PASSWORD "**********"
#define DEVICE_TOKEN "***********"

#define PIR 5

void setup()
{
  Serial.begin(115200);
  pinMode(PIR, INPUT);
  Cloud.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
}

void loop()
{
 Cloud.loop();
 int pirState = digitalRead(PIR) ;
 Cloud.write("V7", pirState);
 delay(10);
}
