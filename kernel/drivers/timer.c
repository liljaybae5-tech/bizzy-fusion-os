/**
 * Bizzy-Fusion OS - Timer Driver Implementation
 */

#include "timer.h"
#include "../io/io.h"
#include "../interrupts/interrupts.h"
#include "../core/kernel.h"

static uint32_t ticks = 0;

/**
 * Initialize timer
 */
void timer_init() {
    kprintf("[TIMER] Initializing system timer...\n");
    
    // Register timer interrupt handler (IRQ 0)
    interrupt_register(0x20, timer_handler);
    
    // Setup timer frequency
    uint32_t divisor = 1193180 / TIMER_FREQUENCY;
    
    // Send commands to PIT (Programmable Interval Timer)
    outb(0x43, 0x36);
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
    
    kprintf("[TIMER] System timer initialized at %d Hz\n", TIMER_FREQUENCY);
}

/**
 * Timer interrupt handler
 */
void timer_handler(uint32_t num) {
    ticks++;
    // Trigger context switch every 20ms
    if (ticks % 20 == 0) {
        // scheduler_yield();
    }
}

/**
 * Get current tick count
 */
uint32_t get_ticks() {
    return ticks;
}

/**
 * Sleep for specified milliseconds
 */
void sleep_ms(uint32_t ms) {
    uint32_t start = ticks;
    while (ticks - start < ms);
}
