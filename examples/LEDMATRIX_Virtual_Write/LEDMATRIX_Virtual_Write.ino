#include <PetalLedmatrix.h>
#include <iotCloud.h>


LEDMATRIX Petal;

uint16_t width = 6;
uint16_t spacer = 0;
String textStr = "";           // String storage (SAFE)
const char* text = "";         // char* for LED matrix

int len = 0;
String vPinRead = "V1";
void displayPrint()
{
  if (len > 3)
  {
//    Serial.println("Received");
    Petal.Ledmatrixscroll(width, spacer, text, 1);
    delay(500);
    Petal.LedMatrixClear();
    delay(50);
  }
  else if (len < 4 && len > 0)
  {
//    Serial.println("Done");
    Petal.LedmatrixShow(0, 0, text, 1);
//    delay(500);
//    Petal.LedMatrixClear();
    delay(50);
  }

}
void setup()
{
  Serial.begin(115200);
  Petal.Ledmatrixinit(7, 1 , 1); //1. brightness 2. Rotation 3. Mirror
  Cloud.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
  Cloud.registerPin(vPinRead, [] (String V)
  {
    textStr = V;                    // store String
    text = textStr.c_str();         // get char pointer
    len = textStr.length();         // correct length

    Serial.println(text);
    Serial.println(len);
  });
}

void loop()
{
  Cloud.loop();
  displayPrint();

}

