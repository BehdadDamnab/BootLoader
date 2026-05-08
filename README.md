# ESP32 Custom Bootloader

A customized second-stage bootloader for ESP32 microcontrollers with boot information display and diagnostic capabilities.

![ESP32](https://img.shields.io/badge/ESP32-Supported-green)
![ESP-IDF](https://img.shields.io/badge/ESP--IDF-v5.5.1-blue)
![License](https://img.shields.io/badge/license-Apache%202.0-blue)

## 📋 Table of Contents

- [Features](#features)
- [Overview](#overview)
- [Hardware Requirements](#hardware-requirements)
- [Installation](#installation)
- [Boot Modes](#boot-modes)
- [Diagnostics](#diagnostics)
- [Project Structure](#project-structure)
- [Usage](#usage)
- [Customization](#customization)
- [Troubleshooting](#troubleshooting)
- [License](#license)

## ✨ Features

### Core Features
- **Custom Boot Banner**: Display bootloader version, chip info, and MAC address on startup
- **Boot Timing Measurement**: Track hardware initialization time
- **GPIO-Based Diagnostics Mode**: Trigger system diagnostics via GPIO 23
- **Comprehensive Diagnostics**: Pre-boot system health checks including:
  - Flash chip detection and verification
  - Partition table integrity check
  - Factory app validation
  - RTC memory testing
- **Modular Code Structure**: Separated into logical components (boot modes, diagnostics, hooks)
- **Enhanced Logging**: Detailed boot process information with formatted output

### Technical Features
- **Hook-Based Architecture**: Uses `bootloader_before_init()` and `bootloader_after_init()` hooks
- **Non-Intrusive Design**: Works alongside standard ESP-IDF bootloader features
- **Factory Reset Support**: Compatible with ESP-IDF's built-in factory reset mechanism
- **Test Firmware Support**: Compatible with ESP-IDF's test partition feature

## 🔍 Overview

This project demonstrates how to customize the ESP32's second-stage bootloader with:
- Custom initialization hooks for displaying system information
- Boot mode detection via GPIO pins
- System diagnostics that can be triggered before application starts
- Modular code organization for easy maintenance and extension

The bootloader maintains full compatibility with ESP-IDF's standard features while adding custom functionality through the hook system.

### Boot Flow

```
Power On/Reset
    ↓
ROM Bootloader (First Stage)
    ↓
Custom Second Stage Bootloader
    ├── bootloader_before_init()
    │   ├── Display Boot Banner
    │   ├── Show Chip Information
    │   └── Display MAC Address
    ├── bootloader_init() [ESP-IDF]
    │   └── Hardware Initialization
    ├── bootloader_after_init()
    │   └── Display Init Time
    ├── Load Partition Table
    ├── Detect Boot Mode (GPIO 23)
    ├── Run Diagnostics (if GPIO 23 active)
    └── Load & Jump to Application
        ↓
    Your Application
```

## 🔧 Hardware Requirements

### Minimum Requirements
- **ESP32** microcontroller (tested on ESP32)
- **Flash**: Minimum 2MB (4MB recommended)
- **Development Board**: Any ESP32 development board

### GPIO Pin Assignments

| GPIO | Function | Active State | Note |
|------|----------|--------------|------|
| GPIO 23 | Diagnostics Mode | HIGH (pulled down) | Triggers system diagnostics |

**Note**: The code uses a pull-down configuration on GPIO 23. When GPIO 23 is HIGH (not connected to GND), diagnostics mode is triggered.

### Wiring for Diagnostics Mode

To trigger diagnostics mode:
- **Normal Boot**: Leave GPIO 23 floating or pull to GND
- **Diagnostics Mode**: Pull GPIO 23 to HIGH (3.3V) or leave floating with internal pull-down disabled

## 📦 Installation

### Prerequisites

1. **ESP-IDF v5.5.1 or later**
   ```bash
   # Windows
   cd %USERPROFILE%
   git clone -b v5.5.1 --recursive https://github.com/espressif/esp-idf.git esp/esp-idf
   cd esp/esp-idf
   install.bat
   export.bat
   
   # Linux/Mac
   git clone -b v5.5.1 --recursive https://github.com/espressif/esp-idf.git ~/esp/esp-idf
   cd ~/esp/esp-idf
   ./install.sh
   source export.sh
   ```

2. **Python 3.8+** (included with ESP-IDF)
3. **Git**

### Installation Steps

**Method: Direct ESP-IDF Component Replacement**

This bootloader replaces the default ESP-IDF bootloader components.

```bash
# Navigate to ESP-IDF bootloader directory
cd %IDF_PATH%\components\bootloader\subproject\main  # Windows
# OR
cd $IDF_PATH/components/bootloader/subproject/main    # Linux/Mac

# Backup original files (recommended)
mkdir backup
cp *.c *.h CMakeLists.txt backup/

# Clone or copy the custom bootloader files
# Copy these files to the current directory:
# - bootloader_start.c
# - bootloader_hooks.c
# - bootloader_hooks.h
# - boot_mode.c
# - boot_mode.h
# - boot_diagnostics.c
# - boot_diagnostics.h
# - CMakeLists.txt
```

### Build and Flash

```bash
# Navigate to your ESP-IDF project
cd /path/to/your/project

# Clean build (recommended after modifying bootloader)
idf.py fullclean

# Build the project
idf.py build

# Flash to ESP32
idf.py -p COM3 flash monitor  # Windows
# OR
idf.py -p /dev/ttyUSB0 flash monitor  # Linux/Mac
```

### Configuration

The bootloader can be configured via `idf.py menuconfig`:

```bash
idf.py menuconfig

# Key configuration options:
# Bootloader config
#   ├── Bootloader log verbosity → Info or Debug
#   └── Bootloader watchdog timeout → 15000 ms (recommended)
```

## 🚀 Boot Modes

This bootloader supports two boot modes:

### 1. Normal Mode (Default)

**Trigger**: GPIO 23 is LOW (connected to GND or pulled down)

**Behavior**:
- Displays custom boot banner with bootloader version (2.1.2)
- Shows ESP32 chip information and revision
- Displays device MAC address
- Reports hardware initialization time in microseconds
- Proceeds with normal partition selection and boot

**Console Output**:
```
==========================================
             SMART BOOTLOADER v2.1.2         
==========================================
Chip: ESP32 rev v3.1
MAC: 24:6f:28:a1:b2:c3
----------------------------------------
Hardware init: 15234 us
► NORMAL BOOT MODE
I (29) boot: ESP-IDF v5.5.1-dirty 2nd stage bootloader
...
```

**Use Case**: Regular device operation

---

### 2. Diagnostics Mode

**Trigger**: GPIO 23 is HIGH (not connected to GND or floating with pull-down disabled)

**Behavior**:
- Displays boot banner and chip information (same as normal mode)
- Detects diagnostics mode via GPIO 23
- Runs comprehensive system diagnostics:
  - **Flash Chip Detection**: Verifies flash manufacturer and device ID
  - **Partition Table Check**: Validates partition table and displays partition information
  - **Factory App Validation**: Verifies factory partition firmware integrity
  - **RTC Memory Test**: Tests RTC memory read/write functionality
- Displays diagnostic summary with pass/fail counts
- Waits 5 seconds before continuing normal boot

**Console Output**:
```
==========================================
             SMART BOOTLOADER v2.1.2         
==========================================
Chip: ESP32 rev v3.1
MAC: 24:6f:28:a1:b2:c3
----------------------------------------
Hardware init: 15234 us
► FACTORY RESET MODE DETECTED

==========================================
       SYSTEM DIAGNOSTICS MODE            
==========================================

[1/4] Flash Chip Detection..........  PASS
      Manufacturer: 0xEF
      Device ID: 0x4016
[2/4] Partition Table...............  PASS
      Factory: 0x00010000 (1024 KB)
      OTA_0:   0x00110000 (1024 KB)
      OTA_1:   0x00210000 (1024 KB)
[3/4] Factory App Validation........  PASS
      Magic: 0xE9
      Segments: 8
[4/4] RTC Memory Test...............  PASS

==========================================
          DIAGNOSTIC SUMMARY              
==========================================
   PASSED:   4                            
   FAILED:   0                            
==========================================

   All systems operational

Continuing boot in 5 seconds...
```

**Use Case**:
- Pre-deployment testing
- Manufacturing quality control
- Field diagnostics
- Troubleshooting hardware issues
- Verifying system health

---

### Additional ESP-IDF Built-in Modes

The bootloader also retains ESP-IDF's standard boot features (configured via `idf.py menuconfig`):

- **Factory Reset**: Via configured GPIO and hold time (if enabled in menuconfig)
- **Test Firmware Boot**: Via configured GPIO and hold time (if enabled in menuconfig)

## 🏗️ Project Structure

```
components/bootloader/subproject/main/
├── bootloader_start.c       # Main bootloader entry point (modified ESP-IDF file)
├── bootloader_hooks.c       # Custom initialization hooks implementation
├── bootloader_hooks.h       # Hook function declarations
├── boot_mode.c             # Boot mode detection logic (GPIO 23)
├── boot_mode.h             # Boot mode type definitions
├── boot_diagnostics.c      # Diagnostic tests implementation
├── boot_diagnostics.h      # Diagnostic function declarations
├── CMakeLists.txt          # Build configuration
└── Kconfig                 # Configuration options (welcome message)
```

### Key Files

#### `bootloader_start.c`
The main bootloader entry point. Modified to:
- Call custom hooks (`bootloader_before_init()`, `bootloader_after_init()`)
- Detect boot mode via GPIO
- Trigger diagnostics when requested
- Maintain compatibility with ESP-IDF standard features

#### `bootloader_hooks.c`
Implements the initialization hooks:
- **`bootloader_before_init()`**: Displays boot banner, chip info, MAC address, starts timing
- **`bootloader_after_init()`**: Reports hardware initialization time

#### `boot_mode.c`
Boot mode detection:
- Configures GPIO 23 with pull-down
- Reads GPIO state to determine boot mode
- Returns `BOOT_MODE_NORMAL` or `BOOT_MODE_DIAGNOSTICS`

#### `boot_diagnostics.c`
Implements system diagnostics:
- Flash chip detection and validation
- Partition table integrity check
- Factory partition validation
- RTC memory read/write test
- Formatted diagnostic output

### Execution Timeline

```
┌─────────────────────────────────────────────────────────┐
│ call_start_cpu0()                                       │
│  ├── bootloader_before_init()         [bootloader_hooks.c]
│  │    ├── Display banner                               │
│  │    ├── Show chip info & MAC                         │
│  │    └── Start timing                                 │
│  ├── bootloader_init()                [ESP-IDF core]   │
│  │    └── Initialize hardware                          │
│  ├── bootloader_after_init()          [bootloader_hooks.c]
│  │    └── Report init time                             │
│  ├── Load partition table             [ESP-IDF core]   │
│  ├── detect_boot_mode()               [boot_mode.c]    │
│  │    └── Read GPIO 23                                 │
│  ├── run_diagnostics()                [boot_diagnostics.c]
│  │    └── If diagnostics mode triggered               │
│  └── bootloader_utility_load_boot_image() [ESP-IDF]   │
└─────────────────────────────────────────────────────────┘
```

## 💻 Usage

### Basic Usage

1. **Flash the bootloader** (included when you flash your project):
   ```bash
   idf.py flash
   ```

2. **Monitor boot process**:
   ```bash
   idf.py monitor
   ```

3. **Normal Boot**:
   - Simply power on or reset the device
   - Ensure GPIO 23 is LOW (connected to GND or left floating with pull-down)

4. **Diagnostics Mode**:
   - Connect GPIO 23 to HIGH (3.3V) before powering on
   - Or configure your hardware to pull GPIO 23 HIGH
   - Reset the device
   - View diagnostic output on serial monitor at 115200 baud

### Example Serial Output

**Normal Boot:**
```
==========================================
             SMART BOOTLOADER v2.1.2         
==========================================
Chip: ESP32 rev v3.1
MAC: 24:6f:28:a1:b2:c3
----------------------------------------
I (28) HOOK: This hook is called BEFORE bootloader initialization
I (37) boot: chip revision: v3.1
...
I (52) HOOK: This hook is called AFTER bootloader initialization
Hardware init: 15234 us
I (58) boot: Enabling RNG early entropy source...
► NORMAL BOOT MODE
I (64) boot: Partition Table:
I (67) boot: ## Label            Usage          Type ST Offset   Length
I (75) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (82) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (90) boot:  2 factory          factory app      00 00 00010000 00100000
I (97) boot: End of partition table
...
```

**Diagnostics Mode:**
```
==========================================
             SMART BOOTLOADER v2.1.2         
==========================================
Chip: ESP32 rev v3.1
MAC: 24:6f:28:a1:b2:c3
----------------------------------------
Hardware init: 15456 us
► FACTORY RESET MODE DETECTED

==========================================
       SYSTEM DIAGNOSTICS MODE            
==========================================

[1/4] Flash Chip Detection..........  PASS
      Manufacturer: 0xEF
      Device ID: 0x4016
[2/4] Partition Table...............  PASS
      Factory: 0x00010000 (1024 KB)
      OTA_0:   0x00110000 (1024 KB)
[3/4] Factory App Validation........  PASS
      Magic: 0xE9
      Segments: 8
[4/4] RTC Memory Test...............  PASS

==========================================
          DIAGNOSTIC SUMMARY              
==========================================
   PASSED:   4                            
   FAILED:   0                            
==========================================

   All systems operational

Continuing boot in 5 seconds...
```

## 🔍 Diagnostics

The diagnostic system performs four comprehensive tests when triggered via GPIO 23:

### Test 1: Flash Chip Detection

**Purpose**: Verify the SPI flash chip is properly connected and responding

**What it checks**:
- Reads flash chip device ID from ROM structure
- Validates manufacturer ID
- Validates device ID
- Ensures flash is not returning invalid values (0x00 or 0xFF)

**Pass Criteria**: Valid manufacturer and device ID detected

**Output**:
```
[1/4] Flash Chip Detection..........  PASS
      Manufacturer: 0xEF
      Device ID: 0x4016
```

---

### Test 2: Partition Table Integrity

**Purpose**: Verify partition table is valid and accessible

**What it checks**:
- Partition table loaded successfully
- Factory partition exists and has valid offset
- OTA partitions detected (if present)
- Partition sizes are reported

**Pass Criteria**: Valid partition table with factory partition

**Output**:
```
[2/4] Partition Table...............  PASS
      Factory: 0x00010000 (1024 KB)
      OTA_0:   0x00110000 (1024 KB)
      OTA_1:   0x00210000 (1024 KB)
```

---

### Test 3: Factory App Validation

**Purpose**: Verify the factory partition contains valid firmware

**What it checks**:
- Factory partition offset is valid
- Image header can be read
- Image magic byte is correct (0xE9)
- Segment count is valid
- Image verification passes

**Pass Criteria**: Factory partition contains valid ESP32 application image

**Output**:
```
[3/4] Factory App Validation........  PASS
      Magic: 0xE9
      Segments: 8
```

**Possible Results**:
- **PASS**: Valid firmware found
- **FAIL**: Firmware corrupted or invalid
- **SKIP**: No factory partition defined

---

### Test 4: RTC Memory Test

**Purpose**: Verify RTC memory (persistent across reboots) is functioning

**What it checks**:
- Writes test pattern (0xDEADBEEF) to RTC register
- Reads back the value
- Compares written and read values

**Pass Criteria**: Written value matches read value

**Output**:
```
[4/4] RTC Memory Test...............  PASS
```

**On Failure**:
```
[4/4] RTC Memory Test...............  FAIL (Expected: 0xDEADBEEF, Got: 0x00000000)
```

---

### Diagnostic Summary

After all tests complete, a summary is displayed:

```
==========================================
          DIAGNOSTIC SUMMARY              
==========================================
   PASSED:   4                            
   FAILED:   0                            
==========================================

   All systems operational
```

Or if failures detected:

```
==========================================
          DIAGNOSTIC SUMMARY              
==========================================
   PASSED:   2                            
   FAILED:   2                            
==========================================

   WARNING: Some tests failed!
```

The bootloader waits 5 seconds before continuing with normal boot, regardless of test results.

## 🎨 Customization

### Changing the Bootloader Version

Edit `bootloader_hooks.c`:

```c
#define BOOTLOADER_VERSION "2.1.2"  // Change this to your version
```

### Customizing the Boot Banner

Edit `bootloader_hooks.c` in the `bootloader_before_init()` function:

```c
esp_rom_printf("\n");
esp_rom_printf("==========================================\n");
esp_rom_printf("     YOUR COMPANY - Device v1.0          \n");  // Customize this
esp_rom_printf("==========================================\n");
```

### Changing the Diagnostics GPIO Pin

Edit `boot_mode.c` to change from GPIO 23 to another pin:

```c
boot_mode_t detect_boot_mode(void)
{
    // Change GPIO 23 to your desired pin (e.g., GPIO 4)
    REG_WRITE(GPIO_ENABLE_W1TC_REG, BIT(4));  // Changed from BIT(23)
    
    // Update the IO_MUX register accordingly
    REG_SET_BIT(PERIPHS_IO_MUX_GPIO4_U, FUN_PD);  // Changed from GPIO23_U
    
    esp_rom_delay_us(10000);
    
    uint32_t gpio_state = REG_READ(GPIO_IN_REG);
    bool gpio4_pressed = (gpio_state & BIT(4));  // Changed from BIT(23)
    
    if (gpio4_pressed) {
        esp_rom_printf("► DIAGNOSTICS MODE DETECTED\n");
        return BOOT_MODE_DIAGNOSTICS;
    }
    
    esp_rom_printf("► NORMAL BOOT MODE\n");
    return BOOT_MODE_NORMAL;
}
```

### Adding More Diagnostic Tests

Edit `boot_diagnostics.c` to add custom tests:

```c
void run_diagnostics(const bootloader_state_t *bs)
{
    // ... existing tests ...
    
    // Add your custom test
    esp_rom_printf("[5/5] Custom Hardware Test..........  ");
    
    // Your test logic here
    bool test_passed = perform_custom_test();
    
    if (test_passed) {
        esp_rom_printf("PASS\n");
        passed++;
    } else {
        esp_rom_printf("FAIL\n");
        failed++;
    }
    
    // Update summary counts
}
```

### Adding Additional Boot Modes

1. **Define new boot mode** in `boot_mode.h`:
   ```c
   typedef enum {
       BOOT_MODE_NORMAL = 0,
       BOOT_MODE_DIAGNOSTICS = 1,
       BOOT_MODE_SAFE = 2,        // New mode
       BOOT_MODE_RECOVERY = 3     // New mode
   } boot_mode_t;
   ```

2. **Add detection logic** in `boot_mode.c`:
   ```c
   // Check multiple GPIOs for different modes
   bool gpio23_high = (gpio_state & BIT(23));
   bool gpio22_high = (gpio_state & BIT(22));
   
   if (gpio23_high && gpio22_high) {
       return BOOT_MODE_RECOVERY;
   } else if (gpio23_high) {
       return BOOT_MODE_DIAGNOSTICS;
   } else if (gpio22_high) {
       return BOOT_MODE_SAFE;
   }
   return BOOT_MODE_NORMAL;
   ```

3. **Handle new mode** in `bootloader_start.c`:
   ```c
   switch (mode) 
   {        
       case BOOT_MODE_DIAGNOSTICS:
           run_diagnostics(bs);
           break;
       
       case BOOT_MODE_SAFE:
           esp_rom_printf("► SAFE MODE - Booting factory partition\n");
           boot_index = 0;  // Force factory partition
           break;
       
       case BOOT_MODE_RECOVERY:
           // Your recovery logic
           break;
           
       case BOOT_MODE_NORMAL:
       default:
           break;
   }
   ```

### Customizing Diagnostic Delay

Edit `boot_diagnostics.c` at the end of `run_diagnostics()`:

```c
esp_rom_printf("Continuing boot in 10 seconds...\n");  // Changed message
esp_rom_delay_us(10000000);  // Changed from 5 to 10 seconds
```

### Adding Custom System Information

Edit `bootloader_hooks.c` to display additional information:

```c
void bootloader_before_init(void) {
    // ... existing code ...
    
    // Add custom hardware version from GPIO
    uint8_t hw_version = read_hardware_version_pins();
    esp_rom_printf("Hardware Version: v%d.%d\n", hw_version >> 4, hw_version & 0xF);
    
    // Add build date
    esp_rom_printf("Build: %s %s\n", __DATE__, __TIME__);
    
    esp_rom_printf("----------------------------------------\n");
}
```

## 🐛 Troubleshooting

### Bootloader Watchdog Resets

**Symptom**: Device resets during bootloader with error `rst:0x10 (RTCWDT_RTC_RESET)`

**Cause**: Bootloader is taking too long (usually due to long delays in diagnostics)

**Solution**:
1. Increase watchdog timeout:
   ```bash
   idf.py menuconfig
   # Bootloader config → Bootloader watchdog timeout → 15000
   ```
2. Or reduce the delay in `boot_diagnostics.c` (change from 5 seconds to less)

### Custom Messages Not Appearing

**Symptom**: Boot looks normal, no custom banner or timing information

**Solution**:
```bash
# Clean and rebuild
idf.py fullclean
idf.py build
idf.py flash
```

Make sure you've replaced the files in the correct location: `$IDF_PATH/components/bootloader/subproject/main/`

### Diagnostics Mode Not Triggering

**Symptom**: Pulling GPIO 23 HIGH doesn't trigger diagnostics

**Checks**:
1. Verify GPIO 23 is actually HIGH (3.3V) - use a multimeter
2. Check the code is using pull-down (`FUN_PD`) correctly
3. Add debug output in `boot_mode.c`:
   ```c
   uint32_t gpio_state = REG_READ(GPIO_IN_REG);
   esp_rom_printf("GPIO state: 0x%08x, GPIO23: %d\n", 
                  gpio_state, (gpio_state & BIT(23)) ? 1 : 0);
   ```

### Serial Monitor Shows Garbage

**Symptom**: Unreadable characters in serial output

**Solution**:
- Set baud rate to **115200** (ESP32 bootloader default)
- In ESP-IDF monitor: This is done automatically
- In other terminals: Set manually

### Compilation Errors

**Symptom**: Build fails with errors about undefined functions or missing headers

**Common Issues**:
1. **Missing files**: Ensure all 7 files are copied (`bootloader_start.c`, `bootloader_hooks.c`, `bootloader_hooks.h`, `boot_mode.c`, `boot_mode.h`, `boot_diagnostics.c`, `boot_diagnostics.h`)
2. **Wrong ESP-IDF version**: Use v5.5.1 or later
3. **CMakeLists.txt mismatch**: Ensure it lists all source files

### GPIO 23 Conflicts with Other Hardware

**Symptom**: GPIO 23 is already used by your hardware design

**Solution**:
Change to a different GPIO pin - see the Customization section above for how to modify the diagnostic trigger GPIO.

## 📚 Additional Resources

- [ESP-IDF Bootloader Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/bootloader.html)
- [ESP32 Technical Reference Manual](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)
- [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/index.html)
- [Custom Bootloader Example (ESP-IDF)](https://github.com/espressif/esp-idf/tree/master/examples/custom_bootloader)

## 📄 License

This project is licensed under the Apache License 2.0 - see the LICENSE file for details.

## 👤 Author

**Behdad Damnab**
- GitHub: [@BehdadDamnab](https://github.com/BehdadDamnab)
