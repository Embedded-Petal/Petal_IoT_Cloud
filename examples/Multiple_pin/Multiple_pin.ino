#include <iotCloud.h>
#include <FastLED.h>

#define WIFI_SSID     "**************"
#define WIFI_PASSWORD  "**************"
#define DEVICE_TOKEN  "*******************"

#define LED_PIN 32
#define NUM_LEDS 8
CRGB leds[NUM_LEDS];

//#define UPDATES_PER_SECOND 100


#define button 16
#define LED1 13
#define LED2 12
#define LED3 14
#define LED4 27
#define pot 34

int LEDState1;
int LEDState2;
int LEDState3;
int LEDState4;
// RGB CACHE
int r = 0, g = 0, b = 0;
String receivedata = "", previousData = "";
void rgb()
{
  auto c = Cloud.readRGB("V5");
  // Only update if changed
  if (c.r != r || c.g != g || c.b != b) {
    r = c.r;
    g = c.g;
    b = c.b;
    Serial.printf("RGB => %d, %d, %d\n", r, g, b);
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(r, g, b); // Set the LED color using HSV (Hue, Saturation, Brightness)
    }
    FastLED.show();  // Update the LED strip
    delay(50);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Version - 1.2");
  // Initialize the RGB LED strip
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(button , INPUT);
  pinMode(pot, INPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  Cloud.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
  Cloud.registerPin("V0", [](String v) {
    LEDState1 = v.toInt();
  });
  Cloud.registerPin("V1", [](String v) {
    LEDState2 = v.toInt();
  });
  Cloud.registerPin("V2", [](String v) {
    LEDState3 = v.toInt();
  });
  Cloud.registerPin("V3", [](String v) {
    LEDState4 = v.toInt();
  });
  Cloud.registerPin("V6", [](String a)
  {
    receivedata = a;
    receivedata;
    Serial.println("Received Data - " + receivedata);
  });
}

void loop() {
  Cloud.loop();
  if (digitalRead(button) == 1)
  {
    Cloud.write("V4", 1);
    //Serial.println("call");
  }
  else {
    Cloud.write("V4", 0);
  }

  if (LEDState1 == 1 ) digitalWrite(LED1, HIGH);
  else digitalWrite(LED1, LOW);
  if (LEDState2 == 1) digitalWrite(LED2, HIGH);
  else digitalWrite(LED2, LOW);
  if (LEDState3 == 1) digitalWrite(LED3, HIGH);
  else digitalWrite(LED3, LOW);
  if (LEDState4 == 1) digitalWrite(LED4, HIGH);
  else digitalWrite(LED4, LOW);
  rgb();
  if (receivedata != previousData)
  {
    Cloud.write("V7", receivedata);
    Serial.println("Data Received " + receivedata);
    previousData = receivedata;
  }
  int a = analogRead(pot);
  if (a > 10 )
  {
    Cloud.write("V8", a);
   // Serial.println("Pot - " + String(a));
    delay(20);
  }

}
