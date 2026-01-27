#include <Petal_IoT_Cloud.h>

#define WIFI_SSID "*******"
#define WIFI_PASSWORD "**********"
#define DEVICE_TOKEN "***********"

#define LED4 27
#define LED3 14
#define LED2 12
#define LED1 13
int LED1State = 0, LED2State = 0, LED3State = 0, LED4State = 0;
void setup()
{
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  Cloud.begin(WIFI_SSID,WIFI_PASSWORD,DEVICE_TOKEN);
  Cloud.registerPin("V9",[](String V)
  {
    LED1State = V.toInt();
    digitalWrite(LED1, LED1State ? HIGH : LOW);
  });
  Cloud.registerPin("V10",[](String V)
  {
    LED2State = V.toInt();
    digitalWrite(LED2, LED2State ? HIGH : LOW);
  });
  Cloud.registerPin("V11",[](String V)
  {
    LED3State = V.toInt();
    digitalWrite(LED3, LED3State ? HIGH : LOW);
  });
  Cloud.registerPin("V12",[](String V)
  {
    LED4State = V.toInt();
    digitalWrite(LED4, LED4State ? HIGH : LOW);
  });
}
void loop()
{
  Cloud.loop();
}
