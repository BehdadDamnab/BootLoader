#ifndef BOOT_MODE_H
#define BOOT_MODE_H

typedef enum {
    BOOT_MODE_NORMAL = 0,
    BOOT_MODE_DIAGNOSTICS = 1
} boot_mode_t;

boot_mode_t detect_boot_mode(void);

#endif // BOOT_MODE_H