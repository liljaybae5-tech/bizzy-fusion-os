/**
 * Bizzy-Fusion OS - Kernel Header
 */

#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stddef.h>

// Kernel configuration
#define KERNEL_VERSION "1.0"
#define MAX_PROCESSES 256
#define PROCESS_STACK_SIZE 4096
#define MEMORY_SIZE 0x4000000  // 64MB

// Utility macros
#define NULL 0
#define TRUE 1
#define FALSE 0
#define PANIC(msg) kernel_panic(msg, __FILE__, __LINE__)

// Forward declarations
void kernel_main();
void kernel_panic(const char *message, const char *file, int line);
void clear_screen();
void kprintf(const char *fmt, ...);
void disable_interrupts();
void enable_interrupts();
void halt_cpu();

#endif // KERNEL_H
