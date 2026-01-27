#include <Petal_IoT_Cloud.h>


#define WIFI_SSID     "**************"
#define WIFI_PASSWORD  "**************"
#define DEVICE_TOKEN  "*******************"

#define LED1 13
int LEDState1;

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW);
  Cloud.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
  Cloud.registerPin("V0", [](String v) {
    LEDState1 = v.toInt();
  });
}

void loop() {
  Cloud.loop();
  if (LEDState1 == 1 ) digitalWrite(LED1, HIGH);
  else digitalWrite(LED1, LOW);
}  

