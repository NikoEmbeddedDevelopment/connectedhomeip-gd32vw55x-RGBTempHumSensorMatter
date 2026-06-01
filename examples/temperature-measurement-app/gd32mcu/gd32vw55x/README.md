# CHIP GD32VW55x Temperature Measurement Example

This example demonstrates a Matter **Temperature Sensor** device on the GD32VW553-EVAL board.

## Device Model (Matter)

### Device Type

- Endpoint 1 advertises device type **Temperature Sensor** (`0x0302`, 770).
- Endpoint 0 is the Root Node endpoint.

### Main Cluster

- Endpoint 1 implements **Temperature Measurement** cluster (`0x0402`, 1026) as a **server**.

Key attributes:

- `MeasuredValue` (int16s): temperature in **0.01°C** units.
  - Example: `2500` means **25.00°C**.
  - Negative values are allowed (e.g. `-500` means **-5.00°C**).
- `MinMeasuredValue` / `MaxMeasuredValue` (int16s): valid range in **0.01°C** units.

### Example Behavior

- This example uses a **simulated** sensor in [SensorManager.cpp](src/SensorManager.cpp).
- The firmware periodically updates `MeasuredValue` (default: every ~5 seconds).
- Pressing the **TAMPER/WAKEUP** key triggers an immediate temperature update.
- Pressing **PC14** triggers factory reset.

## Build

This example is built with GN/Ninja and uses the per-example GN args file:

- [args.gni](args.gni)

Typical build flow (Linux shell / build environment):

```sh
source scripts/activate.sh

gn gen out/gd32vw55x-temperature-measurement --args='import("//examples/temperature-measurement-app/gd32mcu/gd32vw55x/args.gni")'
ninja -C out/gd32vw55x-temperature-measurement
```

Build outputs:

- `out/gd32vw55x-temperature-measurement/chip-gd32vw55x-temperature-measurement-example.elf`

## Flashing

Flashing depends on your GD32 toolchain/programmer setup (J-Link/OpenOCD/vendor tools).
Use the generated `.elf` from the build output folder.

## Commissioning

This device supports commissioning over **BLE + Wi-Fi**.

### Pairing (chip-tool)

Build chip-tool (example):

```sh
./scripts/examples/gn_build_example.sh examples/chip-tool out/debug
```

Commission over BLE and provision Wi-Fi:

```sh
./out/debug/chip-tool pairing ble-wifi 1234 <ssid> <password> 20202021 3840
```

Parameters:

1. Node ID: `1234`
2. SSID: `<ssid>`
3. Password: `<password>`
4. Setup PIN code: `20202021`
5. Discriminator: `3840`

## Functional Test

### 1) Confirm device type and cluster presence

```sh
./out/debug/chip-tool descriptor read device-type-list 1234 1
./out/debug/chip-tool descriptor read server-list 1234 1
```

Expected:

- Endpoint 1 includes device type `0x0302`.
- Endpoint 1 server list includes cluster `0x0402`.

### 2) Read temperature attributes

```sh
./out/debug/chip-tool temperaturemeasurement read measured-value 1234 1
./out/debug/chip-tool temperaturemeasurement read min-measured-value 1234 1
./out/debug/chip-tool temperaturemeasurement read max-measured-value 1234 1
```

Interpretation:

- Divide `measured-value` by 100 to get °C.

### 3) Subscribe and observe periodic updates

```sh
./out/debug/chip-tool temperaturemeasurement subscribe measured-value 0 600 1234 1
```

Expected:

- Reports arrive periodically (matching the device update period).
- Pressing **TAMPER/WAKEUP** causes an immediate report.
