# Smart Parking System

An IoT-based smart parking management system built on ESP32 that automates parking access control and real-time slot monitoring.

## Overview

This system uses RFID-based authentication to control a servo-operated gate and monitors four parking slots using IR sensors. Slot availability is displayed on an LCD screen and synced with the Blynk IoT cloud platform for remote monitoring.

## Features

- **RFID Access Control** — Authorized tag scanning opens the gate via servo motor
- **Real-Time Slot Monitoring** — 4 IR sensors detect vehicle presence in each parking slot
- **LCD Display** — 16×2 I2C LCD shows access status and slot occupancy
- **Cloud Connectivity** — Blynk IoT integration with LED widgets for remote slot status
- **WiFi Enabled** — Connects wirelessly for cloud data sync

## Hardware Requirements

| Component            | Quantity | Notes                       |
|----------------------|----------|-----------------------------|
| ESP32 Development Board | 1     |                             |
| MFRC522 RFID Module  | 1        | SPI interface               |
| RFID Tags/Cards      | 2+       |                            |
| SG90 Servo Motor     | 1        | Gate control                |
| IR Obstacle Sensors  | 4        | One per parking slot        |
| 16×2 I2C LCD Display | 1        | I2C address `0x27`          |
| Jumper Wires         | —        |                             |
| Breadboard / PCB     | —        |                             |

## Pin Connections

| ESP32 Pin | Component       |
|-----------|-----------------|
| GPIO 5    | MFRC522 SS (SDA)|
| GPIO 16   | MFRC522 RST     |
| GPIO 13   | Servo Signal    |
| GPIO 36   | IR Sensor 1     |
| GPIO 39   | IR Sensor 2     |
| GPIO 34   | IR Sensor 3     |
| GPIO 35   | IR Sensor 4     |
| GPIO 21   | I2C SDA (LCD)   |
| GPIO 22   | I2C SCL (LCD)   |

*Standard SPI pins: MOSI → GPIO 23, MISO → GPIO 19, SCK → GPIO 18*

## Software Dependencies

Install the following libraries in Arduino IDE or PlatformIO:

- **MFRC522** — RFID reader library (by GithubCommunity)
- **ESP32Servo** — Servo library for ESP32
- **LiquidCrystal_I2C** — I2C LCD library
- **Blynk** — Blynk IoT platform library (by Volodymyr Shymanskyy)

## Setup & Configuration

1. **Clone the repository**
   ```bash
   git clone https://github.com/YOUR_USERNAME/smart-parking-system.git
   ```

2. **Install required libraries** via Arduino Library Manager or PlatformIO

3. **Update credentials** — Edit the following lines in `SmartParkingSystem.cpp`:
   ```cpp
   #define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
   #define BLYNK_TEMPLATE_NAME "YOUR_TEMPLATE_NAME"
   #define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKEN"

   char ssid[] = "YOUR_WIFI_SSID";
   char pass[] = "YOUR_WIFI_PASSWORD";
   ```

4. **Update authorized tag UIDs** — Replace `TAG_1` and `TAG_2` with your RFID tag UIDs:
   ```cpp
   #define TAG_1 "YOUR_TAG_1_UID"
   #define TAG_2 "YOUR_TAG_2_UID"
   ```

5. **Upload the sketch** to your ESP32 board

## How It Works

1. Scan an RFID tag/card on the MFRC522 reader
2. If the tag UID matches an authorized tag:
   - LCD displays "ACCESS GRANTED"
   - Servo opens the gate for 3 seconds
3. If the tag is unauthorized:
   - LCD displays "ACCESS DENIED" for 2 seconds
4. IR sensors continuously monitor each slot and update:
   - LCD display with slot occupancy
   - Blynk app LED widgets (V4–V7) for remote monitoring

## Blynk App Setup

1. Create a new project in the Blynk app with your ESP32
2. Add 4 LED widgets on virtual pins **V4**, **V5**, **V6**, and **V7**
3. Each LED represents a parking slot (Slot 1 → V4, Slot 2 → V5, etc.)

## License

This project is open source and available under the MIT License.

---

*Built with ESP32, Arduino framework, and Blynk IoT Platform.*
