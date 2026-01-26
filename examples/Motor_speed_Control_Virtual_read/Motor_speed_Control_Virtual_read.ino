#include <iotCloud.h>

#define WIFI_SSID     "Petal"
#define WIFI_PASSWORD  "Petal#2024"
#define DEVICE_TOKEN  "108bcfdd8d0b4130bd5dad72894ad8db1769067998308"

#define M1 13
#define M2 12
#define M3 14
#define M4 27

int speed = 0;
String vPinRead = "V4";
String vPinWrite = "V2";

void setup()
{
  Serial.begin(115200);
  Cloud.begin(WIFI_SSID,WIFI_PASSWORD,DEVICE_TOKEN);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(M3, OUTPUT);
  pinMode(M4, OUTPUT);
  Cloud.registerPin(vPinRead,[] (String V)
  {
    speed = V.toInt();
    Serial.println("Received Speed - " + String(speed));
    if(speed > 0 && speed < 256)
    {
      analogWrite(M1, speed);
      analogWrite(M2, 0);
      analogWrite(M3, speed);
      analogWrite(M4, 0);
      Cloud.write(vPinWrite, "Forward");
    }
    else if(speed < 0 && speed > -256)
    {
      speed = (-1 * speed);
      analogWrite(M1, 0);
      analogWrite(M2, speed);
      analogWrite(M3, 0);
      analogWrite(M4, speed);
      Cloud.write(vPinWrite, "Backward");
    }
    else if(speed == 0)
    {
      analogWrite(M1, 0);
      analogWrite(M2, 0);
      analogWrite(M3, 0);
      analogWrite(M4, 0);
      Cloud.write(vPinWrite, "Stop");
    }
    else 
    {
      Cloud.write(vPinWrite, "Speed range -255 to 255");
    }
  });
}

void loop()
{
  Cloud.loop();
}
