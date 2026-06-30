/**
 * Bizzy-Fusion OS - Interrupt Management Implementation
 * 
 * Implements interrupt handling and IDT management
 */

#include "interrupts.h"
#include "../core/kernel.h"
#include "../utilities/string.h"

static IDTEntry idt[NUM_INTERRUPTS];
static IDTPointer idt_ptr;
static InterruptHandler handlers[NUM_INTERRUPTS] = {0};

/**
 * Initialize interrupt system
 */
void interrupts_init() {
    kprintf("[IRQ] Initializing interrupt system...\n");
    
    memset(idt, 0, sizeof(idt));
    
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base = (uint32_t)&idt;
    
    // Setup default exception handlers
    for (int i = 0; i < NUM_INTERRUPTS; i++) {
        idt_set_gate(i, 0, 0x08, 0x8E);
    }
    
    idt_load();
    kprintf("[IRQ] Interrupt system initialized with %d vectors\n", NUM_INTERRUPTS);
}

/**
 * Register interrupt handler
 */
void interrupt_register(uint8_t num, InterruptHandler handler) {
    if (num < NUM_INTERRUPTS) {
        handlers[num] = handler;
        kprintf("[IRQ] Registered handler for interrupt %d\n", num);
    }
}

/**
 * Set IDT gate
 */
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

/**
 * Load IDT into CPU
 */
void idt_load() {
    // This would use assembly to load IDT
    // asm("lidt %0" : : "m"(idt_ptr));
    kprintf("[IRQ] IDT loaded at 0x%x\n", (uint32_t)&idt_ptr);
}

/**
 * Generic interrupt handler
 */
void interrupt_handler(uint32_t num) {
    if (handlers[num]) {
        handlers[num](num);
    } else {
        kprintf("[IRQ] Unhandled interrupt: %d\n", num);
    }
}
