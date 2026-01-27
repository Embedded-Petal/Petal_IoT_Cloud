#include <Petal_IoT_Cloud.h>

#define WIFI_SSID "*******"
#define WIFI_PASSWORD "**********"
#define DEVICE_TOKEN "***********"


#define IR 5
int lastIr = -1;

void setup() {
  Serial.begin(115200);
  pinMode(IR, INPUT);
  Cloud.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
}

void loop() {
  Cloud.loop();
  int irState = digitalRead(IR);
  if (irState != lastIr) {
    Cloud.write("V1", irState ? 1 : 0);
    lastIr = irState;
  }
}
