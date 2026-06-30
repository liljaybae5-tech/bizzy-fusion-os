/**
 * Bizzy-Fusion OS - Keyboard Driver Implementation
 */

#include "keyboard.h"
#include "../io/io.h"
#include "../interrupts/interrupts.h"
#include "../core/kernel.h"

static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static uint32_t buffer_pos = 0;

// Scan code to ASCII conversion table
static const char scancode_table[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', 0, 0, 'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '"', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/', 0, '*', 0, ' '
};

/**
 * Initialize keyboard
 */
void keyboard_init() {
    kprintf("[KEYBOARD] Initializing keyboard driver...\n");
    
    // Register keyboard interrupt handler (IRQ 1)
    interrupt_register(0x21, keyboard_handler);
    
    kprintf("[KEYBOARD] Keyboard driver initialized\n");
}

/**
 * Keyboard interrupt handler
 */
void keyboard_handler(uint32_t num) {
    uint8_t scancode = inb(0x60);
    
    // Only handle key press (bit 7 = 0)
    if (!(scancode & 0x80)) {
        if (scancode < sizeof(scancode_table)) {
            char c = scancode_table[scancode];
            if (c != 0 && buffer_pos < KEYBOARD_BUFFER_SIZE) {
                keyboard_buffer[buffer_pos++] = c;
            }
        }
    }
}

/**
 * Get character from keyboard buffer
 */
char keyboard_getchar() {
    if (buffer_pos > 0) {
        char c = keyboard_buffer[0];
        // Shift buffer
        for (int i = 0; i < buffer_pos - 1; i++) {
            keyboard_buffer[i] = keyboard_buffer[i + 1];
        }
        buffer_pos--;
        return c;
    }
    return 0;
}
