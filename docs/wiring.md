# Wiring

Board: HiLetgo ESP32-DevKitC-32, 38 pins, USB-C (see [board.md](board.md)). Pin labels checked against the [product photo](https://m.media-amazon.com/images/I/51nghNDy4hL._AC_SL1100_.jpg).

**The pin labels are printed on the _back_ of the board.** Flipping the board over to read them swaps left and right, so there are two views below. Both are drawn with the **antenna end at the top** and the **USB-C port at the bottom**. Pin 1 is at the antenna end on both sides.

Labels match the silkscreen: `P32` = GPIO32, `SVP`/`SVN` = GPIO36/39, `SD0`–`SD3`/`CMD`/`CLK` = GPIO6–11 (flash, don't use).

### Front view (chip side up, as it sits on a breadboard)

```
                ┌──────────────────────┐
                │     ▲ antenna ▲      │
                │    ESP32-WROOM-32    │
                │                      │
    OLED VCC ▶ ─┤3V3   1        1   GND├─ ◀ OLED GND
               ─┤EN    2        2   P23├─
               ─┤SVP   3        3   P22├─ ◀ OLED SCL
               ─┤SVN   4        4    TX├─
               ─┤P34   5        5    RX├─
               ─┤P35   6        6   P21├─ ◀ OLED SDA
    BUTTON A ▶ ─┤P32   7        7   GND├─
    BUTTON B ▶ ─┤P33   8        8   P19├─
               ─┤P25   9        9   P18├─
               ─┤P26  10        10   P5├─
               ─┤P27  11        11  P17├─
               ─┤P14  12        12  P16├─
               ─┤P12  13        13   P4├─
  BUTTON GND ▶ ─┤GND  14        14   P0├─
               ─┤P13  15        15   P2├─ ◀ onboard LED
               ─┤SD2  16        16  P15├─
               ─┤SD3  17        17  SD1├─
               ─┤CMD  18        18  SD0├─
               ─┤5V   19        19  CLK├─
                │                      │
                │ [EN] ┌─────┐ [BOOT]  │
                └───────┤USB-C├────────┘
```

### Back view (flipped over, where you can read the labels)

```
                ┌──────────────────────┐
                │   ▲ antenna end ▲    │
                │(labels printed here) │
                │                      │
    OLED GND ▶ ─┤GND   1        1   3V3├─ ◀ OLED VCC
               ─┤P23   2        2    EN├─
    OLED SCL ▶ ─┤P22   3        3   SVP├─
               ─┤TX    4        4   SVN├─
               ─┤RX    5        5   P34├─
    OLED SDA ▶ ─┤P21   6        6   P35├─
               ─┤GND   7        7   P32├─ ◀ BUTTON A
               ─┤P19   8        8   P33├─ ◀ BUTTON B
               ─┤P18   9        9   P25├─
               ─┤P5   10        10  P26├─
               ─┤P17  11        11  P27├─
               ─┤P16  12        12  P14├─
               ─┤P4   13        13  P12├─
               ─┤P0   14        14  GND├─ ◀ BUTTON GND
 onboard LED ▶ ─┤P2   15        15  P13├─
               ─┤P15  16        16  SD2├─
               ─┤SD1  17        17  SD3├─
               ─┤SD0  18        18  CMD├─
               ─┤CLK  19        19   5V├─
                │                      │
                │       ┌─────┐        │
                └───────┤USB-C├────────┘
```

**Quick orientation check:** the row of pins that has **3V3 at the antenna end and 5V at the USB end** is the one with P32, P33 and the GND for the buttons.

## Ground pins

There are **3 GND pins**, all connected to each other, so use whichever is closest:
- In the 3V3 row: pin 14, between P12 and P13
- In the other row: pin 1, at the antenna end
- In the other row: pin 7, between P21 and P19

## Connections

| Part | Part pin | Board label | Row / pin # | Status |
|---|---|---|---|---|
| Button A | leg 1 | P32 | 3V3 row, pin 7 | ✅ wired |
| Button A | leg 2 | GND | 3V3 row, pin 14 | ✅ wired |
| Button B | leg 1 | P33 | 3V3 row, pin 8 | ✅ wire now |
| Button B | leg 2 | GND | shared GND rail | ✅ wire now |
| OLED SSD1306 | VCC | 3V3 | 3V3 row, pin 1 | later |
| OLED SSD1306 | GND | GND | other row, pin 1 | later |
| OLED SSD1306 | SDA | P21 | other row, pin 6 | later |
| OLED SSD1306 | SCL | P22 | other row, pin 3 | later |
| Onboard LED | — | P2 | built in (blue) | — |

Buttons use the ESP32's internal pull-ups, so they don't need resistors. A button has no polarity, so either leg can go to GND. For a 4-leg tactile button, use two legs on **opposite corners**, because the legs on the same side are often connected internally.

Both buttons can share one GND pin. On a breadboard, run that GND to a rail and connect each button to the rail.

## Pins to avoid

| Labels | Why |
|---|---|
| CLK, SD0, SD1, SD2, SD3, CMD | Wired to the onboard flash (GPIO6–11). Using them crashes the board. |
| P34, P35, SVP, SVN | Input only, with **no** internal pull-up, so a button there needs an external resistor |
| P0, P2, P5, P12, P15 | Strapping pins that affect boot. P12 is the worst one: holding it HIGH at boot breaks flash. |
| TX, RX | USB serial: used for uploading and `pio device monitor` |
| 5V | USB 5 V. **Never** connect it to the OLED or a GPIO. |

## Breadboard tip

This board is wide. On a standard breadboard it covers every hole on one side, so you have nowhere to plug in wires. Either straddle **two breadboards** side by side, or use female jumper wires straight onto the pins.
