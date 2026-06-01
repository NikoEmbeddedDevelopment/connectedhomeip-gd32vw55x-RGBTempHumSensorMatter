# Matter GD32VW553 RGB Lighting + Temperature/Humidity Sensor Example

A combined Matter device running on the **GD32VW553-EVAL** board that exposes a
dimmable RGB light, a temperature sensor, and a humidity sensor — all in a single
firmware image and Matter node.

---

## Table of Contents

- [Hardware Overview](#hardware-overview)
  - [GD32VW553 SoC](#gd32vw553-soc)
  - [GD30TSHT30 Temperature & Humidity Sensor](#gd30tsht30-temperature--humidity-sensor)
  - [WS2812 Environment LED Strip](#ws2812-environment-led-strip)
- [Board Connections (Wiring)](#board-connections-wiring)
- [Matter Device Model](#matter-device-model)
- [Building](#building)
- [Flashing](#flashing)
- [Commissioning](#commissioning)
- [Cluster Control](#cluster-control)

---

## Hardware Overview

### GD32VW553 SoC

The **GD32VW553** is a 32-bit RISC-V microcontroller from GigaDevice with integrated
Wi-Fi (802.11 b/g/n) and Bluetooth 5.0 (BLE). Key specs relevant to this example:

| Feature | Detail |
|---------|--------|
| Core | RISC-V (Bumblebee N307), 160 MHz |
| Wi-Fi | 802.11 b/g/n 2.4 GHz |
| BLE | Bluetooth 5.0 LE |
| Flash | 2 MB on-chip |
| RAM | 448 KB SRAM |
| Peripherals used | I2C0, TIMER2/CH3, DMA CH3, 2× PWM LED, 2× GPIO button |

The evaluation board **GD32VW553H-EVAL** breaks out all peripherals and includes an
LCD display, two user LEDs, two user buttons, and expansion headers.

### GD30TSHT30 Temperature & Humidity Sensor

The **GD30TSHT30** is GigaDevice's version of the SHT30 sensor from Sensirion.

| Property | Value |
|----------|-------|
| Communication | I2C (fast mode, up to 400 kHz) |
| I2C address | `0x44` (7-bit, ADDR pin = LOW) |
| Temperature range | −40 °C … +125 °C |
| Temperature accuracy | ±0.3 °C |
| Humidity range | 0 … 100 %RH |
| Humidity accuracy | ±2 %RH |
| Operating voltage | 2.4 V … 5.5 V |
| Measurement mode | Periodic, high repeatability, 1 measurement/s |

In this firmware the sensor is polled every **1 second** (`SENSOR_POLL_INTERVAL_MS`).
Temperature is reported in units of **0.01 °C** and humidity in **0.01 %RH** as
required by the Matter specification.

### WS2812 Environment LED Strip

A strip of **10 WS2812B** addressable RGB LEDs visualises the current environment:

| Measurement | LED behaviour |
|-------------|---------------|
| Temperature ≤ 20 °C | Blue |
| Temperature = 25 °C | Green |
| Temperature ≥ 30 °C | Red (interpolated between colours) |
| Humidity ≤ 40 %RH | Dim (low brightness) |
| Humidity ≥ 75 %RH | Full brightness (interpolated) |

The strip is driven by **TIMER2 CH3 PWM + DMA CH3** for a CPU-free bit-banging
approach on GPIO **PB2** (AF3).

---

## Board Connections (Wiring)

### GD30TSHT30 → GD32VW553H-EVAL

```
GD30TSHT30 Pin    GD32VW553 Pin    Notes
─────────────────────────────────────────────────────────
VDD               3.3 V            Board 3.3 V rail
GND               GND              Common ground
SCL               PA2              I2C0_SCL, AF4, open-drain
SDA               PA3              I2C0_SDA, AF4, open-drain
ADDR              GND              Sets I2C address to 0x44
```

> Both SCL and SDA require **4.7 kΩ pull-up resistors to 3.3 V** if not already
> present on the sensor breakout board.

```
                    ┌──────────────────────┐
                    │   GD32VW553H-EVAL    │
                    │                      │
GD30TSHT30          │  PA2 ─── SCL         │
  ┌───────┐   SCL ──┤  PA3 ─── SDA         │
  │  VDD  ├─ 3.3V   │                      │
  │  GND  ├─ GND    │  3.3V ─── VDD        │
  │  SCL  ├─────────┤  GND  ─── GND/ADDR   │
  │  SDA  ├─────────┤                      │
  │  ADDR ├─ GND    │  PB2 ─── DIN         │
  └───────┘         └──────────────────────┘
                             │
                             │ PB2 (AF3)
                             ▼
                    WS2812B LED Strip
                    ┌────┬────┬────┬────┐
                    │LED1│LED2│ …  │LED10│
                    └────┴────┴────┴────┘
                     DIN connected to PB2
                     VCC → 5 V (external supply recommended)
                     GND → common GND
```

### On-board LEDs and Buttons

| Signal | Board Component | GPIO | Function |
|--------|----------------|------|----------|
| `LIGHT_LED` | LED2 | Board LED | Reflects Matter OnOff cluster state |
| `SYSTEM_STATE_LED` | LED3 | Board LED | BLE advertising / commissioning status |
| `APP_LIGHT_BUTTON` | USER_BTN1 | Button 1 | Toggle light On/Off |
| `APP_FUNCTION_BUTTON` | USER_BTN2 | Button 2 | Factory reset (hold 6 s) |

---

## Matter Device Model

This firmware presents a **single Matter node** with three endpoints:

| Endpoint | Device Type | Clusters (server) |
|----------|------------|-------------------|
| 0 | Root Node | Basic Information, General Commissioning, … |
| 1 | Dimmable Light | OnOff, LevelControl, ColorControl |
| 2 | Temperature Sensor | TemperatureMeasurement |
| 3 | Humidity Sensor | RelativeHumidityMeasurement |

Default commissioning parameters (see `AppConfig.h`):

| Parameter | Value |
|-----------|-------|
| Setup PIN code | `20202021` |
| Discriminator | `3840` |
| Commissioning mode | BLE (default) |

---

## Building

### Prerequisites

```sh
# From the connectedhomeip repository root:
source scripts/activate.sh
```

Requires the GD32 RISC-V GCC toolchain and the GD32 SDK (see `args.gni` for paths).

### Build commands

```sh
cd examples/lighting-app/gd32mcu/gd32vw55x
gn gen out
ninja -C out
```

Build outputs (inside `out/`):

- `lighting_app.elf` — main application
- `gd32vw55x_mbl.elf` — Matter bootloader (MBL)

To rebuild from scratch:

```sh
rm -rf out/
gn gen out
ninja -C out
```

### Build options (args.gni)

| Variable | Default | Description |
|----------|---------|-------------|
| `TEMP_SENSOR_ENABLED` | `1` | `0` = simulation mode (no hardware needed) |
| `SENSOR_POLL_INTERVAL_MS` | `1000` | Sensor polling period in ms |
| `MAX_CONSECUTIVE_FAILURES` | `10` | I2C failures before reporting INVALID |

---

## Flashing

Use **J-Link** or **GD-Link** with OpenOCD or the GD32 All-In-One Programmer.

Example with J-Link + OpenOCD:

```sh
openocd -f interface/jlink.cfg -f target/gd32vw55x.cfg \
  -c "program out/gd32vw55x_mbl.elf verify reset" \
  -c "program out/lighting_app.elf verify reset exit"
```

Flash the bootloader first, then the application.

---

## Commissioning

The device advertises over **BLE** by default. `SYSTEM_STATE_LED` (LED3) blinks
during BLE advertising.

### Using chip-tool

Build chip-tool if not already available:

```sh
./scripts/examples/gn_build_example.sh examples/chip-tool out/debug
```

Commission over BLE and provision Wi-Fi credentials:

```sh
./out/debug/chip-tool pairing ble-wifi 1234 <SSID> <PASSWORD> 20202021 3840
```

| Argument | Value | Description |
|----------|-------|-------------|
| Node ID | `1234` | Arbitrary node ID you assign |
| SSID | `<SSID>` | Your Wi-Fi network name |
| PASSWORD | `<PASSWORD>` | Your Wi-Fi password |
| Setup PIN | `20202021` | Matches `AppConfig.h` |
| Discriminator | `3840` | Matches `AppConfig.h` |

> **Raspberry Pi BLE tip:** If BLE pairing fails on Raspberry Pi 4, run:
> ```sh
> sudo btmgmt -i hci0 power off
> sudo btmgmt -i hci0 bredr off
> sudo btmgmt -i hci0 power on
> ```

---

## Cluster Control

After successful commissioning (node ID `1234` in these examples):

### Light (Endpoint 1)

```sh
# Turn on / off
./out/debug/chip-tool onoff on  1234 1
./out/debug/chip-tool onoff off 1234 1

# Set brightness (0–254)
./out/debug/chip-tool levelcontrol move-to-level 128 0 0 0 1234 1

# Set hue and saturation
./out/debug/chip-tool colorcontrol move-to-hue-and-saturation 0 200 0 0 0 1234 1
```

### Temperature Sensor (Endpoint 2)

```sh
# Read current temperature (divide by 100 for °C)
./out/debug/chip-tool temperaturemeasurement read measured-value 1234 2

# Subscribe to periodic updates
./out/debug/chip-tool temperaturemeasurement subscribe measured-value 0 60 1234 2
```

### Humidity Sensor (Endpoint 3)

```sh
# Read current humidity (divide by 100 for %RH)
./out/debug/chip-tool relativehumiditymeasurement read measured-value 1234 3

# Subscribe to periodic updates
./out/debug/chip-tool relativehumiditymeasurement subscribe measured-value 0 60 1234 3
```

### Physical button

Press **USER_BTN1** at any time to toggle the light On/Off locally (reflects in
Matter cluster state automatically).
