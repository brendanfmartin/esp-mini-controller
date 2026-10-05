// Button test: a click on either button toggles the LED, press/release logged to serial.
//
// Wiring: each button has one leg on its GPIO, the other on GND.
// The internal pull-up holds the pin HIGH; pressing pulls it LOW.

#include <Arduino.h>

static const uint8_t LED_PIN = 2;  // blue onboard LED on most ESP32 DevKits
static const uint32_t DEBOUNCE_MS = 15;

struct Button {
  const char *name;
  uint8_t pin;
  bool pressed;
  bool lastReading;
  uint32_t lastChangeMs;
};

static Button buttons[] = {
    {"A", 32, false, HIGH, 0},
    {"B", 33, false, HIGH, 0},
};
static const size_t NUM_BUTTONS = sizeof(buttons) / sizeof(buttons[0]);

static uint32_t pressCount = 0;
static bool ledOn = false;

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("\n--- ESP Mini: button test ---");
  for (size_t i = 0; i < NUM_BUTTONS; i++) {
    Serial.printf("Button %s on GPIO%d (to GND)\n", buttons[i].name, buttons[i].pin);
    pinMode(buttons[i].pin, INPUT_PULLUP);
  }
  Serial.printf("LED on GPIO%d\n", LED_PIN);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  const uint32_t now = millis();
  for (size_t i = 0; i < NUM_BUTTONS; i++) {
    Button &b = buttons[i];
    const bool reading = digitalRead(b.pin);

    if (reading != b.lastReading) {
      b.lastReading = reading;
      b.lastChangeMs = now;
    }
    if (now - b.lastChangeMs < DEBOUNCE_MS) continue;

    const bool isDown = (reading == LOW);
    if (isDown == b.pressed) continue;

    b.pressed = isDown;
    if (b.pressed) {
      pressCount++;
      ledOn = !ledOn;
      digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
      Serial.printf("%s pressed  (#%lu) -> LED %s\n", b.name, pressCount, ledOn ? "on" : "off");
    } else {
      Serial.printf("%s released\n", b.name);
    }
  }
}
