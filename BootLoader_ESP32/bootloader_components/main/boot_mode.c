
#include "soc/gpio_reg.h"
#include "soc/io_mux_reg.h"
#include <stdbool.h>
#include "esp_rom_sys.h"
#include "esp_rom_sys.h"
#include "boot_mode.h"

boot_mode_t detect_boot_mode(void)
{
    // Configure GPIOs as inputs with pullups
    REG_WRITE(GPIO_ENABLE_W1TC_REG, BIT(23));
    
    // Enable pullups
    REG_SET_BIT(PERIPHS_IO_MUX_GPIO23_U, FUN_PD);
    
    // Small delay for pins to settle
    esp_rom_delay_us(10000);
    
    // Read GPIO states
    uint32_t gpio_state = REG_READ(GPIO_IN_REG);
    bool gpio23_pressed = (gpio_state & BIT(23));
    
    if (gpio23_pressed) {
        esp_rom_printf("► FACTORY RESET MODE DETECTED\n");
        return BOOT_MODE_DIAGNOSTICS;
    }
    
    esp_rom_printf("► NORMAL BOOT MODE\n");
    return BOOT_MODE_NORMAL;
}
