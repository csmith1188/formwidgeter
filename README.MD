# Formwidgeter

**Formwidgeter** is a software/firmware combination for Arduino that enables network-enabled Arduino platforms to interface with **Formbar** without requiring firmware reflashing. All credentials (e.g., Access Point SSID, password, Formbar server address, and API key) are saved to the first 320 bytes of the device's EEPROM (64 bytes × 5 parameters).

---

## 🚀 Features

- Configure network and API credentials without reflashing
- EEPROM-based storage for persistent settings
- Works with standard Arduino sketches

---

## 🔧 Flashing Formwidgeter to an Arduino Device

1. Copy all `.ino` files to your Arduino **Sketch** folder (except `widget.ino`).
2. Open your main sketch and:
   - Copy required function calls from `widget.ino` into your `setup()` and `loop()` functions.
3. Upload your sketch to the Arduino as usual.

---

## 🔄 Updating Credentials

1. Run the configuration tool:
   - Preferred: `app.exe`
   - Alternative (requires Python): `app.py`
2. **Close any open Serial Monitor** that might be using the device.
3. Identify your device's **Serial Port** (install necessary drivers if it doesn’t show up).
4. Set the **baud rate** (default: `115200`).
5. Click **Connect**.
6. Enter and update values (e.g., SSID, password, server address, API key) using the provided UI.
7. Optionally, you can interact directly through the Serial Port as needed.

---

## 📦 EEPROM Layout

- 5 configurable parameters
- 64 bytes per parameter
- Stored in the first 320 bytes of EEPROM

---

## 💡 Notes

- Always close Serial Monitors before connecting with the configuration tool.
- Using `app.exe` ensures better cross-platform compatibility and user experience.

---

## 🛠 Requirements

- Arduino-compatible board with EEPROM
- Windows (for `app.exe`) or Python 3 (for `app.py`)
- Arduino IDE (for flashing)
- Serial drivers for your device (if needed)

---

## 📄 License

[MIT License](LICENSE)

---

## 🙌 Contributions

Feel free to open issues or submit pull requests! Help make Formwidgeter even better.

