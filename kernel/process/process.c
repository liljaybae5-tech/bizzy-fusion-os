/**
 * Bizzy-Fusion OS - Process Management Implementation
 * 
 * Implements process creation, management, and lifecycle
 */

#include "process.h"
#include "../core/kernel.h"
#include "../utilities/string.h"

// Process table
static ProcessControlBlock process_table[256];
static uint32_t process_count = 0;
static uint32_t next_pid = 1;

/**
 * Initialize process management
 */
void process_init() {
    kprintf("[PROCESS] Initializing process manager...\n");
    memset(process_table, 0, sizeof(process_table));
    process_count = 0;
    next_pid = 1;
    kprintf("[PROCESS] Process manager ready\n");
}

/**
 * Create a new process
 * Returns PID of new process, or -1 on failure
 */
uint32_t process_create(const char *name, void (*entry)()) {
    if (process_count >= 256) {
        kprintf("[PROCESS] ERROR: Maximum process limit reached\n");
        return -1;
    }
    
    ProcessControlBlock *pcb = &process_table[process_count];
    
    pcb->pid = next_pid++;
    pcb->ppid = 0;  // Parent of init is 0
    pcb->state = PROCESS_READY;
    pcb->priority = 10;  // Default priority
    pcb->cpu_ticks = 0;
    pcb->eip = (uint32_t)entry;
    pcb->exit_code = 0;
    
    process_count++;
    
    kprintf("[PROCESS] Created process '%s' with PID %d\n", name, pcb->pid);
    return pcb->pid;
}

/**
 * Terminate a process
 */
void process_terminate(uint32_t pid) {
    for (int i = 0; i < process_count; i++) {
        if (process_table[i].pid == pid) {
            process_table[i].state = PROCESS_TERMINATED;
            kprintf("[PROCESS] Terminated process PID %d\n", pid);
            return;
        }
    }
    kprintf("[PROCESS] ERROR: Process %d not found\n", pid);
}

/**
 * Get process control block by PID
 */
ProcessControlBlock* process_get(uint32_t pid) {
    for (int i = 0; i < process_count; i++) {
        if (process_table[i].pid == pid) {
            return &process_table[i];
        }
    }
    return NULL;
}

/**
 * Wait for process to complete
 */
void process_wait(uint32_t pid) {
    ProcessControlBlock *pcb = process_get(pid);
    if (!pcb) {
        kprintf("[PROCESS] ERROR: Process %d not found\n", pid);
        return;
    }
    
    while (pcb->state != PROCESS_TERMINATED) {
        // Busy wait - in real OS would use sleep
    }
}

/**
 * Context switch between processes
 */
void process_switch(ProcessControlBlock *old, ProcessControlBlock *new) {
    if (old) {
        old->state = PROCESS_READY;
    }
    
    new->state = PROCESS_RUNNING;
    new->cpu_ticks++;
    
    // Update page tables for memory isolation
    // asm("mov %0, %%cr3" : : "r"(new->cr3));
}
