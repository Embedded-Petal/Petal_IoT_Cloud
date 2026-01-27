#include <Petal_IoT_Cloud.h>

#define WIFI_SSID "*******"
#define WIFI_PASSWORD "**********"
#define DEVICE_TOKEN "***********"

#define FlameSensor 23

void setup()
{
  Serial.begin(115200);
  pinMode(FlameSensor, INPUT);
  Cloud.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
}

void loop()
{
  Cloud.loop();
  Cloud.write("V0", digitalRead(FlameSensor) ? 1 : 0);
}
