/**
 * Bizzy-Fusion OS - I/O Operations Implementation
 */

#include "io.h"
#include <stdarg.h>

static uint16_t cursor_x = 0;
static uint16_t cursor_y = 0;
static const uint16_t VGA_WIDTH = 80;
static const uint16_t VGA_HEIGHT = 25;
static uint16_t *video_memory = (uint16_t*)0xB8000;  // VGA memory address

/**
 * Read byte from I/O port
 */
uint8_t inb(uint16_t port) {
    uint8_t value;
    // asm("inb %w1, %b0" : "=a"(value) : "d"(port));
    return value;
}

/**
 * Write byte to I/O port
 */
void outb(uint16_t port, uint8_t value) {
    // asm("outb %b0, %w1" : : "a"(value), "d"(port));
}

/**
 * Read word from I/O port
 */
uint16_t inw(uint16_t port) {
    uint16_t value;
    // asm("inw %w1, %w0" : "=a"(value) : "d"(port));
    return value;
}

/**
 * Write word to I/O port
 */
void outw(uint16_t port, uint16_t value) {
    // asm("outw %w0, %w1" : : "a"(value), "d"(port));
}

/**
 * Read long from I/O port
 */
uint32_t inl(uint16_t port) {
    uint32_t value;
    // asm("inl %w1, %0" : "=a"(value) : "d"(port));
    return value;
}

/**
 * Write long to I/O port
 */
void outl(uint16_t port, uint32_t value) {
    // asm("outl %0, %w1" : : "a"(value), "d"(port));
}

/**
 * Clear the screen
 */
void clear_screen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        video_memory[i] = (0x07 << 8) | ' ';
    }
    cursor_x = 0;
    cursor_y = 0;
}

/**
 * Put a character on screen
 */
void putchar(char c) {
    if (c == '\n') {
        cursor_y++;
        cursor_x = 0;
    } else if (c == '\t') {
        cursor_x += 4;
    } else {
        uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
        video_memory[pos] = (0x07 << 8) | c;
        cursor_x++;
    }
    
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
    
    if (cursor_y >= VGA_HEIGHT) {
        cursor_y = VGA_HEIGHT - 1;
        // Scroll screen
    }
}

/**
 * Print a string
 */
void puts(const char *str) {
    while (*str) {
        putchar(*str++);
    }
}

/**
 * Formatted print (kernel printf)
 */
void kprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    
    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 'd': {
                    int num = va_arg(args, int);
                    char buf[16];
                    itoa(num, buf, 10);
                    puts(buf);
                    break;
                }
                case 'x': {
                    int num = va_arg(args, int);
                    char buf[16];
                    itoa(num, buf, 16);
                    puts("0x");
                    puts(buf);
                    break;
                }
                case 's': {
                    char *str = va_arg(args, char*);
                    puts(str);
                    break;
                }
                case 'c': {
                    char c = (char)va_arg(args, int);
                    putchar(c);
                    break;
                }
                case '%':
                    putchar('%');
                    break;
            }
        } else if (*fmt == '\\') {
            fmt++;
            switch (*fmt) {
                case 'n':
                    putchar('\n');
                    break;
                case 't':
                    putchar('\t');
                    break;
                default:
                    putchar(*fmt);
            }
        } else {
            putchar(*fmt);
        }
        fmt++;
    }
    
    va_end(args);
}
