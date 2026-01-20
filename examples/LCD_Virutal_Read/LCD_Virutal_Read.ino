#include <iotCloud.h>
#include <LiquidCrystal.h>

#define WIFI_SSID "Petal"
#define WIFI_PASSWORD "Petal#2024"
#define DEVICE_TOKEN "455af741ac7449e88eab613ea44f80f81768800404446"

// ---------------- LCD CONFIG ----------------
// RS, EN, D4, D5, D6, D7
LiquidCrystal lcd(13, 12, 14, 27, 26, 25);

// ---------------- CLOUD STRING ----------------
String lcdText = "";

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);

  // LCD Initialize
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IoT Cloud");
  lcd.setCursor(0, 1);
  lcd.print("Waiting...");

  delay(2000);
  lcd.clear();

  // IoT Cloud Start
  Cloud.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
  Cloud.registerPin("V14", [](String V) {
    lcdText = V;
    Serial.print("Received String: ");
    Serial.println(lcdText);

    lcd.clear();

    // Line 1
    lcd.setCursor(0, 0);
    lcd.print(lcdText.substring(0, 16));

    // Line 2 (if text > 16 chars)
    if (lcdText.length() > 16) {
      lcd.setCursor(0, 1);
      lcd.print(lcdText.substring(16, 32));
    }
  });
}

// ---------------- LOOP ------

void loop() {
  Cloud.loop();
}