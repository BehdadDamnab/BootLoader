#include "esp_log.h"

#include "soc/efuse_reg.h"
#include "esp_chip_info.h"
#include "esp_cpu.h"


#define BOOTLOADER_VERSION "2.1.0"

static uint32_t boot_start_time;


void bootloader_hooks_include(void){
}


void bootloader_before_init(void) {
    /* Keep in my mind that a lot of functions cannot be called from here
     * as system initialization has not been performed yet, including
     * BSS, SPI flash, or memory protection. */
    
    ESP_LOGI("HOOK", "This hook is called BEFORE bootloader initialization");

    boot_start_time = esp_cpu_get_cycle_count();

    
    esp_rom_printf("\n");
    esp_rom_printf("==========================================\n");
    esp_rom_printf("             SMART BOOTLOADER v%s         \n", BOOTLOADER_VERSION);
    esp_rom_printf("==========================================\n");


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

    uint32_t init_time = esp_cpu_get_cycle_count() - boot_start_time;
    uint32_t init_time_us = init_time / 160;  // cycles / (160 MHz) = microseconds
    esp_rom_printf("Hardware init: %lu us\n", init_time_us);

    ESP_LOGI("HOOK", "This hook is called AFTER bootloader initialization");    

}