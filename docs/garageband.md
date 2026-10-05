# GarageBand (iPhone)

## Connecting

Don't pair from **Settings → Bluetooth**. In GarageBand, open any instrument, then go to **Settings (gear) → Advanced → Bluetooth MIDI Devices → ESP Mini MIDI**. Accept the pairing popup the first time. The onboard LED goes solid once it's connected and blinks slowly while it waits.

## Troubleshooting

**It connects (or keeps trying) but no notes play.** The serial monitor shows `notify: No clients subscribed` on every button press. That means iOS connected but never subscribed to the MIDI characteristic. This happened after reflashing firmware with a different Bluetooth layout on the same address, and iOS kept stale state. The fix:

1. **Settings → Bluetooth** → ⓘ next to ESP Mini MIDI → **Forget This Device**
2. **Restart the iPhone.** Forgetting the device alone isn't enough.
3. Press **EN** on the board, then connect from GarageBand again.

Button presses log `subscribers: N` over serial. If N is 0, notes aren't reaching the phone.
