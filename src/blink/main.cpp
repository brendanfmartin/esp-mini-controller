// Blink the onboard LED and print chip info over serial — a first sanity check for the board.

#include <Arduino.h>

static const uint8_t LED_PIN = 2;  // blue onboard LED on most ESP32 DevKits
static const uint32_t BLINK_MS = 500;

void setup() {
  Serial.begin(115200);
  delay(200);

  Serial.println("\n--- ESP Mini: blink ---");
  Serial.printf("Chip:     %s rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
  Serial.printf("Cores:    %d @ %lu MHz\n", ESP.getChipCores(), ESP.getCpuFreqMHz());
  Serial.printf("Flash:    %lu MB\n", ESP.getFlashChipSize() / (1024 * 1024));
  Serial.printf("MAC:      %012llX\n", ESP.getEfuseMac());
  Serial.printf("SDK:      %s\n", ESP.getSdkVersion());

  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  static bool on = false;
  on = !on;
  digitalWrite(LED_PIN, on ? HIGH : LOW);
  Serial.println(on ? "LED on" : "LED off");
  delay(BLINK_MS);
}
