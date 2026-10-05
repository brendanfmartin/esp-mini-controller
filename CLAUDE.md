# ESP Mini MIDI Controller

PlatformIO project: ESP32 (ESP32-D0WD-V3, 4MB flash, `esp32dev`) acting as a Bluetooth LE MIDI controller for GarageBand on iPhone. Two buttons send notes; an SSD1306 128x64 I2C OLED shows status.

- Wiring lives in [docs/wiring.md](docs/wiring.md) (pinout diagram + connection table). It's the source of truth: whenever a pin assignment changes in code, update the diagram, the table, and the status column in the same change.
- Board is on `/dev/cu.usbserial-0001` (CP2102). Full chip details in [docs/board.md](docs/board.md).
- Envs in `platformio.ini`, selected by `build_src_filter`, one folder each under `src/`: `blink`, `button` (default), `midi`. Build one with `pio run -e <env>`.

## Rules

- **Never flash the board.** Do not run `pio run -t upload`, `esptool.py write_flash`, or any other command that writes to the device. The user runs uploads themselves. Building (`pio run`) to check that code compiles is fine.
- Don't run `pio device monitor` either — it needs an interactive terminal. Ask the user to paste serial output if needed.
