# Board

**Product:** HiLetgo ESP32-DevKitC-32 (38-pin, USB-C, sold as a 3-pack on Amazon), a clone of Espressif's ESP32-DevKitC. Pin layout: [wiring.md](wiring.md).

Read from the chip on 2026-10-04 with PlatformIO's bundled esptool (v4.11.0).

| Property | Value |
|---|---|
| Chip | ESP32-D0WD-V3 |
| Silicon revision | v3.1 |
| Features | WiFi, Bluetooth (Classic + BLE), dual core, 240 MHz |
| Crystal | 40 MHz |
| Flash | 4 MB (manufacturer `0xc4`, device `0x6016`), 3.3 V |
| MAC | `20:9b:a9:88:9b:28` |
| USB-serial bridge | Silicon Labs CP2102 (VID:PID `10C4:EA60`) |
| Serial port (macOS) | `/dev/cu.usbserial-0001` |
| PlatformIO board | `esp32dev` |
| Onboard LED | GPIO2 (blue). Confirmed with the blink sketch; the official Espressif DevKitC doesn't have one. |

## Re-checking

```sh
# What PlatformIO sees on USB (only identifies the CP2102, not the ESP)
pio device list

# Read the chip itself (read-only, doesn't write anything to the board)
pio pkg exec -p tool-esptoolpy -- esptool.py --port /dev/cu.usbserial-0001 flash_id
```

The `blink` env also prints chip model, revision, cores, flash size and MAC to serial at boot.
