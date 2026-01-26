#include <iotCloud.h>
#include "StepperMotor.h"

#define WIFI_SSID     "Petal"
#define WIFI_PASSWORD  "Petal#2024"
#define DEVICE_TOKEN  "dc971248626d4ef087646621637c4f9d1769065895799"


// ESP32 Pins
#define M1 13
#define M2 12
#define M3 14
#define M4 27


int speeds = 1;
int step = 0;
String vPinRead = "V1";

// Create object
StepperMotor motor(M1, M2, M3, M4, speeds);

void setup() {
  motor.begin();
  Serial.begin(115200);
  Cloud.begin(WIFI_SSID,WIFI_PASSWORD,DEVICE_TOKEN);
  Cloud.registerPin(vPinRead,[] (String V)
  {
    step = V.toInt();
    Serial.println("Received Step - " + String(step));
    if(step > 0)
    {
      Serial.println("forward");
      motor.forwardSteps(step);
    }
    else if (step < 0){
        step = (-1 * step);
        motor.backwardSteps(step);
    }
    else if(step == 0)
    {
      motor.forwardSteps(0);
    }
  });

}

void loop() {
 Cloud.loop();
 
}
