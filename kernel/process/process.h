/**
 * Bizzy-Fusion OS - Process Management Header
 * 
 * Defines process structures and management functions
 */

#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <stddef.h>

// Process states
typedef enum {
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_WAITING,
    PROCESS_TERMINATED
} ProcessState;

// Process control block (PCB)
typedef struct {
    uint32_t pid;              // Process ID
    uint32_t ppid;             // Parent process ID
    ProcessState state;        // Current state
    uint32_t priority;         // Process priority
    uint32_t cpu_ticks;        // CPU time used
    uint32_t stack_base;       // Base of stack
    uint32_t stack_top;        // Top of stack
    uint32_t esp;              // Stack pointer
    uint32_t eip;              // Instruction pointer
    uint32_t cr3;              // Page directory address
    uint32_t exit_code;        // Exit status
} ProcessControlBlock;

// Process management functions
void process_init();
uint32_t process_create(const char *name, void (*entry)());
void process_terminate(uint32_t pid);
void process_switch(ProcessControlBlock *old, ProcessControlBlock *new);
ProcessControlBlock* process_get(uint32_t pid);
void process_wait(uint32_t pid);

#endif // PROCESS_H
