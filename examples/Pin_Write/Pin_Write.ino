#include <Petal_IoT_Cloud.h>


#define WIFI_SSID     "**************"
#define WIFI_PASSWORD  "**************"
#define DEVICE_TOKEN  "*******************"

#define button 16

void setup() {
  Serial.begin(115200);
  pinMode(button , INPUT);
  Cloud.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
}

void loop() {
  Cloud.loop();
  if (digitalRead(button) == 1)
  {
    Cloud.write("V4", 1);
    
  }
}

