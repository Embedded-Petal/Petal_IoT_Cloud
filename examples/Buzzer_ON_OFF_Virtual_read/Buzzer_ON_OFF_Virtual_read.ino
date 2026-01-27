#include <Petal_IoT_Cloud.h>

#define WIFI_SSID  "*******"
#define WIFI_PASSWORD "**********"
#define DEVICE_TOKEN "***********"


#define buzzer 26

int buzzerState = 0;
void setup()
{
  Serial.begin(115200);
  pinMode(buzzer, OUTPUT);
 
  digitalWrite(buzzer, LOW);
  
  Cloud.begin(WIFI_SSID,WIFI_PASSWORD,DEVICE_TOKEN);
  Cloud.registerPin("V9",[](String V)
  {
    buzzerState = V.toInt();
    digitalWrite(buzzer, buzzerState ? HIGH : LOW);
  });

}
void loop()
{
  Cloud.loop();
}
