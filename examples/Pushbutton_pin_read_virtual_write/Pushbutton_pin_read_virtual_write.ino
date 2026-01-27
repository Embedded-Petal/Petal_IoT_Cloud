#include <Petal_IoT_Cloud.h>


#define WIFI_SSID "************"
#define WIFI_PASSWORD "*************"
#define DEVICE_TOKEN "**********************"

#define Button4 36
#define Button3 39
#define Button2 34
#define Button1 35

void setup() {
  Serial.begin(115200);
  pinMode(Button1, INPUT);
  pinMode(Button2, INPUT);
  pinMode(Button3, INPUT);
  pinMode(Button4, INPUT);
  Cloud.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
}

void loop() {
  Cloud.loop();
  if (digitalRead(Button1) == 0)
    Cloud.write("V0", 1);
  else
    Cloud.write("V0", 0);

  if (digitalRead(Button2) == 0)
    Cloud.write("V1", 1);
  else
    Cloud.write("V1", 0);

  if (digitalRead(Button3) == 0)
    Cloud.write("V2", 1);
  else
    Cloud.write("V2", 0);

  if (digitalRead(Button4) == 0)
    Cloud.write("V3", 1);
  else
    Cloud.write("V3", 0);
}
