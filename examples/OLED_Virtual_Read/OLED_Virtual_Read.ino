#include <Petal_IoT_Cloud.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define WIFI_SSID  "*******"
#define WIFI_PASSWORD "**********"
#define DEVICE_TOKEN "***********"

// ---------------- OLED CONFIG ----------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1   // Reset pin not used

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------------- CLOUD STRING ----------------
String oledText = "";

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);

  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("IoT Cloud");
  display.println("Waiting...");
  display.display();

  delay(2000);
  display.clearDisplay();
  display.display();

  // IoT Cloud Start
  Cloud.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);

  // Register Cloud Pin
  Cloud.registerPin("V14", [](String V) {
    oledText = V;

    Serial.print("Received String: ");
    Serial.println(oledText);

    display.clearDisplay();
    display.setCursor(0, 0);

    // Print string (auto wrap)
    display.println(oledText);

    display.display();
  });
}

// ---------------- LOOP ----------------
void loop() {
  Cloud.loop();   // MUST
}
