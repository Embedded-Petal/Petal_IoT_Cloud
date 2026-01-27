#include <Petal_IoT_Cloud.h>


#define WIFI_SSID  "*******"
#define WIFI_PASSWORD "**********"
#define DEVICE_TOKEN "***********"


#define relay1 25
#define relay2 33

int relay1State = 0, relay2State = 0;

void setup()
{
  //Serial.begin(115200);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
 
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  
  Cloud.begin(WIFI_SSID,WIFI_PASSWORD,DEVICE_TOKEN);
  Cloud.registerPin("V9",[](String V)
  {
    relay1State = V.toInt();
    digitalWrite(relay1, relay1State ? HIGH : LOW);
  });
  Cloud.registerPin("V10",[](String V)
  {
    relay2State = V.toInt();
    digitalWrite(relay2, relay2State ? HIGH : LOW);
  });

}
void loop()
{
  Cloud.loop();
}
