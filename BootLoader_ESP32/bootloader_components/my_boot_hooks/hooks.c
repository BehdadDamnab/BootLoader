#include "esp_log.h"

#include "soc/rtc_cntl_reg.h"
#include "soc/efuse_reg.h"

#include "esp_chip_info.h"


#define BOOTLOADER_VERSION "2.0.0"

static uint32_t boot_start_time;
static uint32_t boot_counter = 0;

void bootloader_hooks_include(void){
}


void bootloader_before_init(void) {
    /* Keep in my mind that a lot of functions cannot be called from here
     * as system initialization has not been performed yet, including
     * BSS, SPI flash, or memory protection. */
    ESP_LOGI("HOOK", "This hook is called BEFORE bootloader initialization");

    // boot_start_time = esp_rom_get_time();

    // boot_counter = READ_PERI_REG(RTC_CNTL_STORE1_REG);
    // boot_counter++;
    // WRITE_PERI_REG(RTC_CNTL_STORE1_REG, boot_counter);

    
    esp_rom_printf("\n");
    esp_rom_printf("╔════════════════════════════════════════╗\n");
    esp_rom_printf("║   SMART BOOTLOADER v%s              ║\n", BOOTLOADER_VERSION);
    esp_rom_printf("╚════════════════════════════════════════╝\n");
    // esp_rom_printf("Boot #%d\n", boot_counter);

    // Get chip info
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    uint32_t chip_ver = chip_info.model;
    esp_rom_printf("Chip: ESP32 rev v%d.%d\n", chip_ver / 100, chip_ver % 100);
    
    // Read MAC address
    uint32_t mac_h = REG_READ(EFUSE_BLK0_RDATA1_REG);
    uint32_t mac_l = REG_READ(EFUSE_BLK0_RDATA2_REG);
    esp_rom_printf("MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
        (mac_h >> 8) & 0xff, mac_h & 0xff,
        (mac_l >> 24) & 0xff, (mac_l >> 16) & 0xff,
        (mac_l >> 8) & 0xff, mac_l & 0xff);
    
    esp_rom_printf("----------------------------------------\n");

}

void bootloader_after_init(void) {
    ESP_LOGI("HOOK", "This hook is called AFTER bootloader initialization");
}