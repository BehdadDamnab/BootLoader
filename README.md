# ESP32 Custom Bootloader

A feature-rich second-stage bootloader for ESP32 microcontrollers with advanced boot mode selection, diagnostics, and recovery features.

![ESP32](https://img.shields.io/badge/ESP32-Supported-green)
![ESP-IDF](https://img.shields.io/badge/ESP--IDF-v5.5.1-blue)
![License](https://img.shields.io/badge/license-Apache%202.0-blue)

## 📋 Table of Contents

- [Features](#features)
- [Overview](#overview)
- [Hardware Requirements](#hardware-requirements)
- [Installation](#installation)
- [Boot Modes](#boot-modes)
- [Configuration](#configuration)
- [Usage](#usage)
- [Architecture](#architecture)
- [Diagnostics](#diagnostics)
- [Recovery & Rollback](#recovery--rollback)
- [Customization](#customization)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

## ✨ Features

### Core Features
- **Multiple Boot Modes**: Normal, Safe Mode, Factory Reset, and Diagnostics
- **GPIO-Based Mode Selection**: Hardware switches/buttons trigger different boot behaviors
- **Boot Health Monitoring**: Automatic crash detection and rollback to stable firmware
- **Comprehensive Diagnostics**: Pre-boot system health checks
- **Boot Counter & History**: Track boot attempts and failures in RTC memory
- **Recovery Mechanisms**: Automatic recovery from failed boot attempts
- **Enhanced Logging**: Detailed boot process information with custom banners

### Advanced Features
- **Partition Validation**: Verify firmware integrity before boot
- **Reset Reason Analysis**: Detailed information about why the device restarted
- **Flash Chip Detection**: Identify and verify flash memory
- **Security Status Reporting**: Display secure boot and encryption status
- **Customizable Boot Delays**: Configurable timing for different scenarios
- **RTC Memory Testing**: Verify non-volatile memory integrity

## 🔍 Overview

This custom bootloader enhances the ESP32's second-stage bootloader with advanced features for development, production, and field deployment. It provides multiple boot modes accessible via GPIO pins, comprehensive diagnostics, and automatic recovery mechanisms.

### Boot Flow

```
Power On/Reset
    ↓
ROM Bootloader (First Stage)
    ↓
Custom Second Stage Bootloader
    ├── Display Boot Banner
    ├── Initialize Hardware
    ├── Detect Boot Mode (GPIO)
    ├── Run Diagnostics (if requested)
    ├── Check Boot Health
    ├── Select Partition
    ├── Validate Firmware
    └── Load & Jump to Application
        ↓
    Your Application
```

## 🔧 Hardware Requirements

### Minimum Requirements
- **ESP32** (any variant: ESP32, ESP32-S2, ESP32-S3, ESP32-C3)
- **Flash**: Minimum 2MB (4MB recommended)
- **Development Board**: Any ESP32 development board

### GPIO Pin Assignments

| GPIO | Function | Active State |
|------|----------|--------------|
| GPIO 0 | Safe Mode | LOW (button pressed) |
| GPIO 2 | Factory Reset | LOW (button pressed) |
| GPIO 4 | Diagnostics Mode | LOW (button pressed) |

**Note**: These pins can be customized in the configuration.

### Optional Hardware
- OLED Display (I2C) for visual boot status (future feature)
- External boot mode selector switch
- Status LEDs

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

2. **Python 3.8+** (comes with ESP-IDF)

3. **Git**

### Method 1: Direct ESP-IDF Modification (Quick Testing)

⚠️ **Warning**: This method modifies your ESP-IDF installation. Changes will affect all projects.

```bash
# Navigate to ESP-IDF bootloader directory
cd $IDF_PATH/components/bootloader/subproject/main

# Backup original files
cp bootloader_start.c bootloader_start.c.backup
cp bootloader_hooks.c bootloader_hooks.c.backup
cp bootloader_hooks.h bootloader_hooks.h.backup

# Copy custom bootloader files
# (Replace with your custom files from this repository)
```

### Method 2: Project-Level Component (Recommended)

This method keeps your customizations separate from ESP-IDF:

```bash
# Clone this repository
git clone https://github.com/BehdadDamnab/BootLoader.git
cd BootLoader

# Your project structure should look like:
# BootLoader_ESP32/
# ├── components/
# │   └── bootloader/
# │       └── subproject/
# │           └── main/
# │               ├── bootloader_start.c
# │               ├── bootloader_hooks.c
# │               └── bootloader_hooks.h
# ├── main/
# │   └── main.c
# ├── CMakeLists.txt
# └── sdkconfig

# Build the project
idf.py build

# Flash to ESP32
idf.py -p COM3 flash monitor  # Windows
idf.py -p /dev/ttyUSB0 flash monitor  # Linux
```

### Configuration

```bash
# Configure bootloader options
idf.py menuconfig

# Navigate to:
# Bootloader config
#   ├── Bootloader log verbosity (set to Info or Debug)
#   ├── Bootloader watchdog timeout (15000 ms recommended)
#   └── Custom GPIO pins (if needed)
```

## 🚀 Boot Modes

### 1. Normal Mode (Default)

**Trigger**: No GPIO pins held during boot

**Behavior**:
- Displays boot banner and system information
- Loads the partition selected by OTA data
- Fastest boot time
- Standard operation

**Use Case**: Regular device operation

---

### 2. Safe Mode

**Trigger**: Hold **GPIO 0** (BOOT button) during power-on

**Behavior**:
- Forces boot from factory partition
- Bypasses OTA partition selection
- Adds 3-second delay for user awareness
- Ignores potentially corrupted OTA firmware

**Use Case**:
- Recovery from bad OTA update
- Testing factory firmware
- Known-good configuration needed

**Console Output**:
```
► SAFE MODE DETECTED
Booting factory firmware (safe mode)
```

---

### 3. Factory Reset Mode

**Trigger**: Hold **GPIO 2** during power-on

**Behavior**:
- Erases NVS (Non-Volatile Storage) partition
- Optionally erases other data partitions
- Resets to factory defaults
- Automatically reboots after reset

**Use Case**:
- Clear all user settings
- Prepare device for new user
- Troubleshoot configuration issues

**Console Output**:
```
► FACTORY RESET MODE DETECTED
Erasing user data...
Factory reset complete. Rebooting...
```

---

### 4. Diagnostics Mode

**Trigger**: Hold **GPIO 4** during power-on

**Behavior**:
- Runs comprehensive system diagnostics
- Tests flash, partitions, RTC memory
- Displays security status
- Shows boot history
- Continues normal boot after 5 seconds

**Use Case**:
- Manufacturing testing
- Field diagnostics
- System health verification
- Troubleshooting hardware issues

**Console Output**:
```
╔════════════════════════════════════════╗
║      SYSTEM DIAGNOSTICS MODE           ║
╚════════════════════════════════════════╝

[1/8] Flash Chip Detection..........  ✓ PASS
[2/8] Flash Size Detection..........  ✓ PASS
[3/8] Partition Table...............  ✓ PASS
[4/8] Factory App Validation........  ✓ PASS
[5/8] Last Reset Reason.............  ✓ INFO
[6/8] Security Features.............  ✓ INFO
[7/8] RTC Memory Test...............  ✓ PASS
[8/8] Boot History..................  ✓ INFO

╔════════════════════════════════════════╗
║         DIAGNOSTIC SUMMARY             ║
╠════════════════════════════════════════╣
║  PASSED:   8                           ║
║  FAILED:   0                           ║
╚════════════════════════════════════════╝
```

## ⚙️ Configuration

### Menuconfig Options

Access configuration via:
```bash
idf.py menuconfig
```

#### Important Settings

**Bootloader config → Bootloader log verbosity**
- Options: None, Error, Warning, Info, Debug, Verbose
- Recommended: **Info** (for production), **Debug** (for development)

**Bootloader config → Bootloader watchdog timeout**
- Default: 9000 ms
- Recommended: **15000 ms** (allows time for diagnostics)
- Set to 0 to disable (not recommended for production)

**Bootloader config → GPIO Pin Configuration**
- Factory Reset Pin: GPIO 2
- App Test Pin: GPIO 4  
- Custom pins can be defined in code

### Custom Configuration in Code

Edit `bootloader_start.c`:

```c
// Custom version string
#define BOOTLOADER_VERSION "2.0.0"

// Boot mode GPIO pins
#define BOOT_MODE_SAFE_PIN      0
#define BOOT_MODE_RESET_PIN     2
#define BOOT_MODE_DIAG_PIN      4

// Boot attempt limits for auto-recovery
#define MAX_BOOT_ATTEMPTS       3

// Delays (microseconds)
#define SAFE_MODE_DELAY         3000000  // 3 seconds
#define DIAG_MODE_DELAY         5000000  // 5 seconds
```

## 💻 Usage

### Basic Usage

1. **Flash the bootloader**:
   ```bash
   idf.py flash
   ```

2. **Monitor boot process**:
   ```bash
   idf.py monitor
   ```

3. **To enter Safe Mode**:
   - Hold BOOT button (GPIO 0)
   - Press and release RESET button
   - Keep holding BOOT for 1 second
   - Release BOOT button

4. **To run Diagnostics**:
   - Connect GPIO 4 to GND
   - Reset the device
   - View diagnostic output on serial monitor

### Application Integration

In your main application, mark successful boots:

```c
#include "soc/rtc_cntl_reg.h"

void app_main(void)
{
    // Your initialization code...
    
    // After successful init, clear boot attempt counter
    WRITE_PERI_REG(RTC_CNTL_STORE1_REG, 0);
    ESP_LOGI("APP", "Boot marked as successful");
    
    // Your application code...
}
```

This prevents automatic rollback when your app is running correctly.

## 🏗️ Architecture

### File Structure

```
components/bootloader/subproject/main/
├── bootloader_start.c       # Main bootloader entry point
├── bootloader_hooks.c       # Custom initialization hooks
├── bootloader_hooks.h       # Hook function declarations
└── CMakeLists.txt          # Build configuration
```

### Key Components

#### `bootloader_start.c`
- Main bootloader logic
- Boot mode detection
- Partition selection
- Recovery mechanisms

#### `bootloader_hooks.c`
- `bootloader_before_init()`: Runs before hardware initialization
- `bootloader_after_init()`: Runs after hardware is ready
- Custom boot banner and system info display

#### Boot State Management
Uses RTC memory (survives resets) to track:
- **RTC_CNTL_STORE0_REG**: Total boot counter
- **RTC_CNTL_STORE1_REG**: Failed boot attempts
- **RTC_CNTL_STORE2_REG**: Last booted partition index
- **RTC_CNTL_STORE3_REG**: Available for custom flags

### Execution Timeline

```
┌─────────────────────────────────────────┐
│ call_start_cpu0()                       │
│  ├── bootloader_before_init()          │  ← Custom hook
│  ├── bootloader_init()                 │  ← ESP-IDF hardware init
│  ├── bootloader_after_init()           │  ← Custom hook
│  ├── Load partition table              │
│  ├── detect_boot_mode()                │  ← Read GPIOs
│  ├── selected_boot_partition()         │  ← Apply boot logic
│  ├── check_boot_health()               │  ← Crash detection
│  └── bootloader_utility_load_boot_image() │  ← Load app
└─────────────────────────────────────────┘
```

## 🔍 Diagnostics

### What Gets Tested

1. **Flash Chip Detection**
   - Manufacturer ID
   - Device ID
   - Chip validation

2. **Flash Size Detection**
   - Total flash capacity
   - Usable space verification

3. **Partition Table Integrity**
   - Factory partition presence
   - OTA partition detection
   - Partition offsets and sizes

4. **Firmware Validation**
   - Image header magic byte
   - Segment count
   - Integrity checks

5. **Reset Reason Analysis**
   - Power-on reset
   - Software reset
   - Watchdog triggers
   - Brownout detection

6. **Security Status**
   - Secure boot state
   - Flash encryption state

7. **RTC Memory Test**
   - Read/write verification
   - Data persistence check

8. **Boot History**
   - Total boot count
   - Failed attempts
   - Last boot partition

### Diagnostic Output Example

```
╔════════════════════════════════════════╗
║      SYSTEM DIAGNOSTICS MODE           ║
╚════════════════════════════════════════╝

[1/8] Flash Chip Detection..........  ✓ PASS
      Manufacturer: 0xEF
      Device ID: 0x4016
[2/8] Flash Size Detection..........  ✓ PASS
      Size: 4 MB
[3/8] Partition Table...............  ✓ PASS
      Factory: 0x00010000 (1024 KB)
      OTA_0:   0x00110000 (1024 KB)
      OTA_1:   0x00210000 (1024 KB)
[4/8] Factory App Validation........  ✓ PASS
      Magic: 0xE9
      Segments: 8
[5/8] Last Reset Reason.............  ✓ INFO
      Reason: Power On Reset
[6/8] Security Features.............  ✓ INFO
      Secure Boot: DISABLED
      Flash Encryption: DISABLED
[7/8] RTC Memory Test...............  ✓ PASS
[8/8] Boot History..................  ✓ INFO
      Total Boots: 47
      Failed Attempts: 0

╔════════════════════════════════════════╗
║         DIAGNOSTIC SUMMARY             ║
╠════════════════════════════════════════╣
║  PASSED:   8                           ║
║  FAILED:   0                           ║
╚════════════════════════════════════════╝

✓  All systems operational
```

## 🛡️ Recovery & Rollback

### Automatic Crash Detection

The bootloader monitors boot health using RTC memory:

1. **Boot Attempt Tracking**
   - Increments counter on each boot attempt
   - Resets counter when app marks boot as successful

2. **Failure Detection**
   - If same partition fails to boot 3 times consecutively
   - Automatically rolls back to factory partition

3. **Recovery Process**
   ```
   Boot Attempt 1 → Crash → Attempt 2 → Crash → Attempt 3 → Crash
   → Rollback to Factory Partition
   ```

### Manual Recovery

**Method 1: Safe Mode**
- Hold GPIO 0 during boot
- Boots factory partition

**Method 2: Erase Flash**
```bash
idf.py erase_flash
idf.py flash
```

**Method 3: UART Recovery**
- Use esptool.py to reflash bootloader and app
```bash
esptool.py --chip esp32 --port COM3 erase_flash
esptool.py --chip esp32 --port COM3 write_flash 0x1000 bootloader.bin
esptool.py --chip esp32 --port COM3 write_flash 0x10000 app.bin
```

### Best Practices

1. **Always call `mark_boot_successful()`** in your app after critical init
2. **Monitor watchdog timeouts** - they trigger automatic resets
3. **Test OTA updates** thoroughly before deployment
4. **Keep factory partition** as known-good fallback

## 🎨 Customization

### Custom Boot Banner

Edit `bootloader_hooks.c`:

```c
void bootloader_after_init(void) {
    esp_rom_printf("\n");
    esp_rom_printf("╔════════════════════════════════════════╗\n");
    esp_rom_printf("║   YOUR COMPANY NAME - Device v1.0     ║\n");
    esp_rom_printf("╚════════════════════════════════════════╝\n");
    
    // Display custom device info
    esp_rom_printf("Serial: ABC123456\n");
    esp_rom_printf("Model: ESP32-DevKit\n");
}
```

### Add Custom Boot Mode

1. **Define new GPIO pin**:
   ```c
   #define BOOT_MODE_CUSTOM_PIN  5
   ```

2. **Add detection logic** in `detect_boot_mode()`:
   ```c
   bool gpio5_pressed = !(gpio_state & BIT(5));
   if (gpio5_pressed) {
       return BOOT_MODE_CUSTOM;
   }
   ```

3. **Implement behavior** in `selected_boot_partition()`:
   ```c
   case BOOT_MODE_CUSTOM:
       esp_rom_printf("Custom mode activated!\n");
       // Your custom logic here
       break;
   ```

### Custom Diagnostic Tests

Add to `run_diagnostics()`:

```c
esp_rom_printf("[9/9] Custom Hardware Test.........  ");
// Your test code
if (test_passed) {
    esp_rom_printf("✓ PASS\n");
    passed++;
} else {
    esp_rom_printf("✗ FAIL\n");
    failed++;
}
```

### Hardware-Specific Features

#### Read Hardware Version from GPIOs
```c
static uint8_t read_hardware_version(void) {
    // Configure version pins
    REG_WRITE(GPIO_ENABLE_W1TC_REG, BIT(5) | BIT(18) | BIT(19));
    
    uint32_t gpio_state = REG_READ(GPIO_IN_REG);
    uint8_t version = 0;
    
    version |= (gpio_state & BIT(5)) ? 0 : 1;
    version |= (gpio_state & BIT(18)) ? 0 : 2;
    version |= (gpio_state & BIT(19)) ? 0 : 4;
    
    return version;
}
```

#### Select Firmware Based on Hardware
```c
static int selected_boot_partition(const bootloader_state_t *bs) {
    uint8_t hw_version = read_hardware_version();
    
    if (hw_version == 1) {
        // Boot specific firmware for hardware v1
        return 0;  // Factory
    } else if (hw_version == 2) {
        // Boot specific firmware for hardware v2
        return 1;  // OTA_0
    }
    
    return bootloader_utility_get_selected_boot_partition(bs);
}
```

## 🐛 Troubleshooting

### Bootloader Watchdog Resets

**Symptom**: Device resets during bootloader with error `rst:0x10 (RTCWDT_RTC_RESET)`

**Solution**:
1. Reduce boot delays
2. Increase watchdog timeout:
   ```bash
   idf.py menuconfig
   # Bootloader config → Bootloader watchdog timeout → 15000
   ```
3. Or disable for testing (not recommended for production):
   ```bash
   # Bootloader config → [ ] Enable Bootloader watchdog timer
   ```

### Custom Messages Not Appearing

**Symptom**: Boot looks normal, no custom banner

**Causes**:
1. Changes made to ESP-IDF installation not being used
2. Project using cached bootloader

**Solution**:
```bash
idf.py fullclean
idf.py build
idf.py flash
```

### GPIO Boot Mode Not Working

**Symptom**: Holding GPIO doesn't trigger boot mode

**Checks**:
1. Verify GPIO is pulled LOW (0V) when button pressed
2. Check pullup resistor is enabled in code
3. Verify pin number matches your hardware
4. Add debug output:
   ```c
   uint32_gpio_state = REG_READ(GPIO_IN_REG);
   esp_rom_printf("GPIO state: 0x%08x\n", gpio_state);
   ```

### Boot Loop / Repeated Resets

**Symptom**: Device keeps resetting, never reaches app

**Diagnosis**:
1. Check reset reason in diagnostics mode
2. Look for watchdog timeouts
3. Verify partition table is correct

**Solution**:
```bash
# Erase everything and reflash
idf.py erase_flash
idf.py flash
```

### Partition Not Found

**Symptom**: `ERROR: Partition not found`

**Solution**:
1. Verify partition table in `partitions.csv`
2. Ensure partition offsets don't overlap
3. Reflash partition table:
   ```bash
   idf.py partition-table-flash
   ```

### Serial Monitor Shows Garbage

**Symptom**: Unreadable characters in serial output

**Solution**:
- Set baud rate to **115200** (ESP32 bootloader default)
- Some ESP32 variants use 74880 baud initially

### Application Doesn't Mark Boot Successful

**Symptom**: Automatic rollback keeps happening

**Solution**:
Add to your `app_main()`:
```c
#include "soc/rtc_cntl_reg.h"

void app_main(void) {
    // ... your init code ...
    
    // Mark boot as successful
    WRITE_PERI_REG(RTC_CNTL_STORE1_REG, 0);
    ESP_LOGI("APP", "Boot successful");
}
```

## 📚 Additional Resources

- [ESP-IDF Bootloader Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/bootloader.html)
- [ESP32 Technical Reference Manual](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)
- [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/index.html)
- [ESP32 Partition Tables](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/partition-tables.html)

## 🤝 Contributing

Contributions are welcome! Please follow these guidelines:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Development Guidelines

- Follow ESP-IDF coding style
- Add comments for complex logic
- Test on real hardware before submitting
- Update documentation for new features

## 📄 License

This project is licensed under the Apache License 2.0 - see the [LICENSE](LICENSE) file for details.

## 👤 Author

**Behdad Damnab**
- GitHub: [@BehdadDamnab](https://github.com/BehdadDamnab)

## 🙏 Acknowledgments

- Espressif Systems for ESP-IDF framework
- ESP32 community for documentation and support
- Contributors and testers

## 📝 Version History

### v2.0.0 (Current)
- ✨ Multiple boot mode support
- ✨ Comprehensive diagnostics
- ✨ Automatic crash recovery
- ✨ Boot health monitoring
- ✨ Enhanced logging and banners

### v1.0.0
- 🎉 Initial release
- Basic custom bootloader functionality

---

**Note**: This bootloader is designed for development and educational purposes. For production use, thoroughly test all features and ensure compliance with your security requirements.

For questions, issues, or feature requests, please [open an issue](https://github.com/BehdadDamnab/BootLoader/issues) on GitHub.