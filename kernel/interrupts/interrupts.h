/**
 * Bizzy-Fusion OS - Interrupt Management Header
 * 
 * Interrupt descriptor table and interrupt handling
 */

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>
#include <stddef.h>

#define NUM_INTERRUPTS 256

// Interrupt descriptor structure
typedef struct {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_hi;
} __attribute__((packed)) IDTEntry;

// IDT pointer structure
typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) IDTPointer;

// Interrupt handlers
typedef void (*InterruptHandler)(uint32_t);

// Interrupt functions
void interrupts_init();
void interrupt_register(uint8_t num, InterruptHandler handler);
void interrupt_handler(uint32_t num);
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void idt_load();

#endif // INTERRUPTS_H
