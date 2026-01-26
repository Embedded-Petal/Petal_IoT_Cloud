#include "iotCloud.h"
#include <DHT.h>

#define WIFI_SSID  "*******"
#define WIFI_PASSWORD "**********"
#define DEVICE_TOKEN "***********"

#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void dhtRead() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if(isnan(temperature) || isnan(humidity))
  {
    Serial.println("Failed to read From DHT11 Sensor! ");
    return;
  }

  Cloud.write("V1", temperature);
  Cloud.write("V2", humidity);
  delay(1000);
}
void setup() {
  // put your setup code here, to run once:
  dht.begin();
  Serial.begin(115200);
  Cloud.begin(WIFI_SSID, WIFI_Password, DEVICE_TOKEN);
}

void loop() {
  // put your main code here, to run repeatedly:
  dhtRead();
  Cloud.loop();
}
