#include <stdint.h>

void kernel_main(void) {
    volatile uint16_t *video = (volatile uint16_t*)0xB8000;
    const char *msg = "Hello from Bizzy-Fusion-OS x86_64";
    for (uint64_t i = 0; msg[i] != '\0'; ++i) {
        /* Character in low byte, attribute (light gray on black) in high byte */
        video[i] = (uint16_t)((0x07 << 8) | (uint8_t)msg[i]);
    }

    for (;;)
        __asm__("hlt");
}
