// #include "bootloader_flash.h"

// #include "bootloader_utility.h"
// #include "esp_image_format.h"
// #include "esp_efuse.h"

// #include "boot_diagnostics.h"
// #include "boot_mode.h"


#include "esp_rom_sys.h"
#include "bootloader_utility.h"


void run_diagnostics(const bootloader_state_t *bs)
{
    esp_rom_printf("\n");
    esp_rom_printf("╔════════════════════════════════════════╗\n");
    esp_rom_printf("║      SYSTEM DIAGNOSTICS MODE           ║\n");
    esp_rom_printf("╚════════════════════════════════════════╝\n");
    esp_rom_printf("\n");
    
    uint32_t passed = 0;
    uint32_t failed = 0;
    
    // // ═══════════════════════════════════════════════════
    // // TEST 1: Flash Chip Detection
    // // ═══════════════════════════════════════════════════
    // esp_rom_printf("[1/8] Flash Chip Detection..........  ");
    // uint32_t flash_id = bootloader_read_flash_id();
    
    // if (flash_id != 0 && flash_id != 0xFFFFFFFF) {
    //     esp_rom_printf("✓ PASS\n");
    //     esp_rom_printf("      Manufacturer: 0x%02x\n", (flash_id >> 16) & 0xFF);
    //     esp_rom_printf("      Device ID: 0x%04x\n", flash_id & 0xFFFF);
    //     passed++;
    // } else {
    //     esp_rom_printf("✗ FAIL\n");
    //     failed++;
    // }
    
    // // ═══════════════════════════════════════════════════
    // // TEST 2: Flash Size Detection
    // // ═══════════════════════════════════════════════════
    // esp_rom_printf("[2/8] Flash Size Detection..........  ");
    // uint32_t flash_size = bootloader_flash_get_size();
    
    // if (flash_size > 0) {
    //     esp_rom_printf("✓ PASS\n");
    //     esp_rom_printf("      Size: %d MB\n", flash_size / (1024 * 1024));
    //     passed++;
    // } else {
    //     esp_rom_printf("✗ FAIL\n");
    //     failed++;
    // }
    
    // // ═══════════════════════════════════════════════════
    // // TEST 3: Partition Table Integrity
    // // ═══════════════════════════════════════════════════
    // esp_rom_printf("[3/8] Partition Table...............  ");
    
    // if (bs != NULL && bs->factory.offset != 0) {
    //     esp_rom_printf("✓ PASS\n");
    //     esp_rom_printf("      Factory: 0x%08x (%d KB)\n", 
    //                    bs->factory.offset, bs->factory.size / 1024);
        
    //     if (bs->ota[0].offset != 0) {
    //         esp_rom_printf("      OTA_0:   0x%08x (%d KB)\n",
    //                        bs->ota[0].offset, bs->ota[0].size / 1024);
    //     }
    //     if (bs->ota[1].offset != 0) {
    //         esp_rom_printf("      OTA_1:   0x%08x (%d KB)\n",
    //                        bs->ota[1].offset, bs->ota[1].size / 1024);
    //     }
    //     passed++;
    // } else {
    //     esp_rom_printf("✗ FAIL\n");
    //     failed++;
    // }
    
    // // ═══════════════════════════════════════════════════
    // // TEST 4: Factory Partition Validation
    // // ═══════════════════════════════════════════════════
    // esp_rom_printf("[4/8] Factory App Validation........  ");
    
    // if (bs->factory.offset != 0) {
    //     esp_image_metadata_t data;
    //     esp_err_t err = bootloader_load_image_header(bs->factory.offset, 
    //                                                   &data.image, 
    //                                                   &data.part_header);
        
    //     if (err == ESP_OK) {
    //         esp_rom_printf("✓ PASS\n");
    //         esp_rom_printf("      Magic: 0x%02x\n", data.image.magic);
    //         esp_rom_printf("      Segments: %d\n", data.image.segment_count);
    //         passed++;
    //     } else {
    //         esp_rom_printf("✗ FAIL (Error: %d)\n", err);
    //         failed++;
    //     }
    // } else {
    //     esp_rom_printf("⊘ SKIP (No factory partition)\n");
    // }
    
    // // ═══════════════════════════════════════════════════
    // // TEST 5: Reset Reason Analysis
    // // ═══════════════════════════════════════════════════
    // esp_rom_printf("[5/8] Last Reset Reason.............  ");
    // int reset_reason = esp_rom_get_reset_reason(0);
    
    // esp_rom_printf("✓ INFO\n");
    // esp_rom_printf("      Reason: ");
    // switch (reset_reason) {
    //     case RESET_REASON_CHIP_POWER_ON:
    //         esp_rom_printf("Power On Reset\n");
    //         break;
    //     case RESET_REASON_CORE_SW:
    //         esp_rom_printf("Software Reset\n");
    //         break;
    //     case RESET_REASON_CORE_DEEP_SLEEP:
    //         esp_rom_printf("Deep Sleep Wake\n");
    //         break;
    //     case RESET_REASON_CORE_MWDT0:
    //         esp_rom_printf("Task Watchdog\n");
    //         break;
    //     case RESET_REASON_CORE_MWDT1:
    //         esp_rom_printf("Interrupt Watchdog\n");
    //         break;
    //     case RESET_REASON_CORE_RTC_WDT:
    //         esp_rom_printf("RTC Watchdog\n");
    //         break;
    //     case RESET_REASON_CPU0_MWDT0:
    //         esp_rom_printf("CPU0 Watchdog\n");
    //         break;
    //     case RESET_REASON_SYS_BROWN_OUT:
    //         esp_rom_printf("Brownout Detector\n");
    //         break;
    //     case RESET_REASON_SYS_RTC_WDT:
    //         esp_rom_printf("System RTC Watchdog\n");
    //         break;
    //     default:
    //         esp_rom_printf("Unknown (%d)\n", reset_reason);
    //         break;
    // }
    // passed++;
    
    // // ═══════════════════════════════════════════════════
    // // TEST 6: eFuse & Security Status
    // // ═══════════════════════════════════════════════════
    // esp_rom_printf("[6/8] Security Features.............  ");
    
    // bool secure_boot = esp_efuse_read_field_bit(ESP_EFUSE_ABS_DONE_0);
    // bool flash_encryption = esp_efuse_read_field_bit(ESP_EFUSE_FLASH_CRYPT_CNT);
    
    // esp_rom_printf("✓ INFO\n");
    // esp_rom_printf("      Secure Boot: %s\n", secure_boot ? "ENABLED" : "DISABLED");
    // esp_rom_printf("      Flash Encryption: %s\n", flash_encryption ? "ENABLED" : "DISABLED");
    // passed++;
    
    // // ═══════════════════════════════════════════════════
    // // TEST 7: RTC Memory Test
    // // ═══════════════════════════════════════════════════
    // esp_rom_printf("[7/8] RTC Memory Test...............  ");
    
    // // Write test pattern
    // uint32_t test_pattern = 0xDEADBEEF;
    // WRITE_PERI_REG(RTC_CNTL_STORE3_REG, test_pattern);
    
    // // Read back
    // uint32_t read_back = READ_PERI_REG(RTC_CNTL_STORE3_REG);
    
    // if (read_back == test_pattern) {
    //     esp_rom_printf("✓ PASS\n");
    //     passed++;
    // } else {
    //     esp_rom_printf("✗ FAIL (Expected: 0x%08x, Got: 0x%08x)\n", 
    //                    test_pattern, read_back);
    //     failed++;
    // }
    
    // // ═══════════════════════════════════════════════════
    // // TEST 8: Boot Counter & Health
    // // ═══════════════════════════════════════════════════
    // esp_rom_printf("[8/8] Boot History..................  ");
    
    // uint32_t boot_counter = READ_PERI_REG(RTC_CNTL_STORE0_REG);
    // uint32_t boot_attempts = READ_PERI_REG(RTC_CNTL_STORE1_REG);
    
    // esp_rom_printf("✓ INFO\n");
    // esp_rom_printf("      Total Boots: %d\n", boot_counter);
    // esp_rom_printf("      Failed Attempts: %d\n", boot_attempts);
    // passed++;
    
    // // ═══════════════════════════════════════════════════
    // // Summary
    // // ═══════════════════════════════════════════════════
    // esp_rom_printf("\n");
    // esp_rom_printf("╔════════════════════════════════════════╗\n");
    // esp_rom_printf("║         DIAGNOSTIC SUMMARY             ║\n");
    // esp_rom_printf("╠════════════════════════════════════════╣\n");
    // esp_rom_printf("║  PASSED:  %2d                           ║\n", passed);
    // esp_rom_printf("║  FAILED:  %2d                           ║\n", failed);
    // esp_rom_printf("╚════════════════════════════════════════╝\n");
    
    // if (failed > 0) {
    //     esp_rom_printf("\n⚠  WARNING: Some tests failed!\n");
    // } else {
    //     esp_rom_printf("\n✓  All systems operational\n");
    // }
    
    // esp_rom_printf("\nPress and hold GPIO 4 for 3 seconds to re-run diagnostics\n");
    // esp_rom_printf("Continuing boot in 5 seconds...\n");
    
    // // Wait 5 seconds before continuing
    // esp_rom_delay_us(5000000);
}