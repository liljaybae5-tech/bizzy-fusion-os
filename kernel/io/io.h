/**
 * Bizzy-Fusion OS - I/O Operations Header
 */

#ifndef IO_H
#define IO_H

#include <stdint.h>

// I/O port functions
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t inl(uint16_t port);
void outb(uint16_t port, uint8_t value);
void outw(uint16_t port, uint16_t value);
void outl(uint16_t port, uint32_t value);

// Screen I/O
void clear_screen();
void putchar(char c);
void puts(const char *str);
void kprintf(const char *fmt, ...);

#endif // IO_H
