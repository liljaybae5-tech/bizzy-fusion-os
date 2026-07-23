#include "core/kernel.h"

// Forward declaration so the compiler knows itoa exists before it is called
void itoa(int num, char* str, int base);

// Read a byte from a port
uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

// Write a byte to a port
void outb(uint16_t port, uint8_t value) {
    __asm__ volatile("outb %0, %1" :: "a"(value), "Nd"(port));
}

// Read a word (16 bits) from a port
uint16_t inw(uint16_t port) {
    uint16_t value;
    __asm__ volatile("inw %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

// Write a word (16 bits) to a port
void outw(uint16_t port, uint16_t value) {
    __asm__ volatile("outw %0, %1" :: "a"(value), "Nd"(port));
}

// Read a long (32 bits) from a port
uint32_t inl(uint16_t port) {
    uint32_t value;
    __asm__ volatile("inl %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

// Write a long (32 bits) to a port
void outl(uint16_t port, uint32_t value) {
    __asm__ volatile("outl %0, %1" :: "a"(value), "Nd"(port));
}

// Keep your existing printing or formatting functions here (Lines 60-130+)
// ...
// (The compiler will successfully find itoa() here when it processes your file)


// ============================================================================
// Implementation of itoa for your freestanding kernel
// ============================================================================
void itoa(int num, char* str, int base) {
    int i = 0;
    _Bool isNegative = 0;

    // Handle 0 explicitly
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // Base 10 negative numbers
    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    // Append negative sign
    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}
