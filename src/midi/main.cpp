// ESP Mini MIDI controller: two buttons -> BLE MIDI notes, status on an SSD1306 OLED.
//
// Wiring
//   OLED  GND -> GND, VCC -> 3V3, SDA -> GPIO21, SCL -> GPIO22
//   Button A: GPIO32 <-> GND
//   Button B: GPIO33 <-> GND   (internal pull-ups, so no resistors needed)

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ESP32.h>

// ---- Config -----------------------------------------------------------------

static const char *DEVICE_NAME = "ESP Mini MIDI";
static const uint8_t MIDI_CHANNEL = 1;
static const uint8_t VELOCITY = 100;
static const uint32_t DEBOUNCE_MS = 15;

struct Button {
  uint8_t pin;
  uint8_t note;  // MIDI note number (60 = middle C)
  bool pressed;
  bool lastReading;
  uint32_t lastChangeMs;
};

static Button buttons[] = {
    {32, 60, false, HIGH, 0},  // A: C4
    {33, 67, false, HIGH, 0},  // B: G4
};
static const size_t NUM_BUTTONS = sizeof(buttons) / sizeof(buttons[0]);

// ---- Display ----------------------------------------------------------------

static const uint8_t SCREEN_W = 128;
static const uint8_t SCREEN_H = 64;
static const uint8_t OLED_ADDR = 0x3C;  // some modules use 0x3D

Adafruit_SSD1306 display(SCREEN_W, SCREEN_H, &Wire, -1);
static bool displayOk = false;

// ---- MIDI -------------------------------------------------------------------

BLEMIDI_CREATE_INSTANCE(DEVICE_NAME, MIDI);

static volatile bool bleConnected = false;
static bool needsRedraw = true;
static uint8_t lastNote = 0;
static bool hasLastNote = false;

static void onConnected() {
  bleConnected = true;
  needsRedraw = true;
}

static void onDisconnected() {
  bleConnected = false;
  needsRedraw = true;
}

// ---- Helpers ----------------------------------------------------------------

static String noteName(uint8_t note) {
  static const char *names[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
  return String(names[note % 12]) + String((int)note / 12 - 1);
}

static void drawScreen() {
  if (!displayOk) return;

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // Header: connection status
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(bleConnected ? "BLE: connected" : "BLE: waiting...");
  display.drawFastHLine(0, 10, SCREEN_W, SSD1306_WHITE);

  // Big last-played note in the middle
  display.setTextSize(3);
  display.setCursor(0, 18);
  display.print(hasLastNote ? noteName(lastNote) : "--");

  // Two pads along the bottom, filled while held
  const uint8_t padW = SCREEN_W / 2 - 2;
  const uint8_t padH = 16;
  const uint8_t padY = SCREEN_H - padH;
  for (size_t i = 0; i < NUM_BUTTONS; i++) {
    const uint8_t x = i * (SCREEN_W / 2) + 1;
    const String label = String(char('A' + i)) + " " + noteName(buttons[i].note);
    display.setTextSize(1);
    if (buttons[i].pressed) {
      display.fillRect(x, padY, padW, padH, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    } else {
      display.drawRect(x, padY, padW, padH, SSD1306_WHITE);
      display.setTextColor(SSD1306_WHITE);
    }
    display.setCursor(x + 4, padY + 4);
    display.print(label);
  }

  display.display();
}

static void pollButtons() {
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
    if (isDown) {
      MIDI.sendNoteOn(b.note, VELOCITY, MIDI_CHANNEL);
      lastNote = b.note;
      hasLastNote = true;
      Serial.printf("Button %c down -> note on %s\n", 'A' + (int)i, noteName(b.note).c_str());
    } else {
      MIDI.sendNoteOff(b.note, 0, MIDI_CHANNEL);
      Serial.printf("Button %c up   -> note off\n", 'A' + (int)i);
    }
    needsRedraw = true;
  }
}

// ---- Arduino ----------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  Serial.println("\nESP Mini MIDI starting");

  for (size_t i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttons[i].pin, INPUT_PULLUP);
  }

  Wire.begin(21, 22);
  displayOk = display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  if (!displayOk) {
    Serial.println("SSD1306 not found (check wiring / try address 0x3D). Continuing without display.");
  }

  BLEMIDI.setHandleConnected(onConnected);
  BLEMIDI.setHandleDisconnected(onDisconnected);
  MIDI.begin(MIDI_CHANNEL_OMNI);

  Serial.printf("Advertising as \"%s\"\n", DEVICE_NAME);
}

void loop() {
  MIDI.read();
  pollButtons();

  static bool wasConnected = false;
  if (bleConnected != wasConnected) {
    wasConnected = bleConnected;
    Serial.println(bleConnected ? "BLE connected" : "BLE disconnected");
  }

  if (needsRedraw) {
    needsRedraw = false;
    drawScreen();
  }
}
