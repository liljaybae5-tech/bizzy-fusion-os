/**
 * Bizzy-Fusion OS - Main Kernel Entry Point
 * 
 * This is the main kernel function called by the bootloader.
 * It initializes all kernel subsystems and starts the scheduler.
 */

#include "core/kernel.h"
#include "interrupts/interrupts.h"
#include "process/scheduler.h"
#include "memory/paging.h"
#include "io/io.h"
#include "drivers/keyboard.h"
#include "drivers/timer.h"
#include "utilities/string.h"

// Forward declarations
void print_banner();
void print_system_info();
void initialize_subsystems();

/**
 * Main kernel entry point
 * Called from boot.s after initial setup
 */
void kernel_main() {
    // Clear the screen
    clear_screen();
    
    // Print welcome banner
    print_banner();
    
    // Print system information
    print_system_info();
    
    // Initialize all kernel subsystems
    initialize_subsystems();
    
    // Start the process scheduler
    scheduler_start();
    
    // The kernel never reaches here - scheduler runs indefinitely
    while(1);
}

/**
 * Print the Bizzy-Fusion OS banner
 */
void print_banner() {
    kprintf("\n");
    kprintf("╔════════════════════════════════════════════════════════╗\n");
    kprintf("║                                                        ║\n");
    kprintf("║          BIZZY-FUSION OPERATING SYSTEM v1.0            ║\n");
    kprintf("║              Lightweight, Modular Kernel               ║\n");
    kprintf("║                                                        ║\n");
    kprintf("╚════════════════════════════════════════════════════════╝\n");
    kprintf("\n");
}

/**
 * Print system initialization information
 */
void print_system_info() {
    kprintf("[KERNEL] Initializing Bizzy-Fusion OS...\n");
    kprintf("[KERNEL] System Architecture: i686 (32-bit x86)\n");
    kprintf("[KERNEL] Available Memory: 64 MB\n");
    kprintf("[KERNEL] Max Processes: 256\n");
    kprintf("\n");
}

/**
 * Initialize all kernel subsystems in order
 */
void initialize_subsystems() {
    kprintf("[INIT] Setting up interrupt handlers...\n");
    interrupts_init();
    
    kprintf("[INIT] Initializing memory management...\n");
    paging_init();
    
    kprintf("[INIT] Initializing process management...\n");
    scheduler_init();
    
    kprintf("[INIT] Setting up timer...\n");
    timer_init();
    
    kprintf("[INIT] Initializing keyboard driver...\n");
    keyboard_init();
    
    kprintf("[INIT] All subsystems initialized successfully!\n");
    kprintf("[INIT] Starting process scheduler...\n\n");
}

/**
 * Kernel panic - unrecoverable error
 * Halts the system and displays error message
 */
void kernel_panic(const char *message, const char *file, int line) {
    // Disable interrupts
    disable_interrupts();
    
    // Clear screen and print error
    clear_screen();
    kprintf("\n╔════════════════════════════════════════════════════════╗\n");
    kprintf("║                   KERNEL PANIC                        ║\n");
    kprintf("╚════════════════════════════════════════════════════════╝\n\n");
    
    kprintf("Message: %s\n", message);
    kprintf("Location: %s:%d\n\n", file, line);
    
    kprintf("System halted. Please reboot.\n");
    
    // Halt the CPU
    while(1) {
        halt_cpu();
    }
}
