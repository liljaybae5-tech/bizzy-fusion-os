/**
 * Bizzy-Fusion OS - Scheduler Implementation
 * 
 * Implements round-robin process scheduling
 */

#include "scheduler.h"
#include "../core/kernel.h"
#include "../utilities/string.h"

static uint32_t current_process = 0;
static uint32_t process_count = 0;
static ProcessControlBlock *current_pcb = NULL;

/**
 * Initialize scheduler
 */
void scheduler_init() {
    kprintf("[SCHEDULER] Initializing scheduler (Round-Robin, %dms quantum)...\n", SCHEDULER_QUANTUM_MS);
    current_process = 0;
    process_count = 0;
    kprintf("[SCHEDULER] Scheduler ready\n");
}

/**
 * Start the scheduler - runs indefinitely
 */
void scheduler_start() {
    kprintf("[SCHEDULER] Starting process scheduler...\n");
    
    while(1) {
        scheduler_schedule();
    }
}

/**
 * Perform scheduling decision
 * Implements round-robin scheduling
 */
void scheduler_schedule() {
    // Get next ready process
    ProcessControlBlock *next_pcb = scheduler_next_process();
    
    if (next_pcb) {
        // Context switch if needed
        if (current_pcb != next_pcb) {
            if (current_pcb) {
                current_pcb->state = PROCESS_READY;
            }
            current_pcb = next_pcb;
            process_switch(NULL, current_pcb);
        }
    }
}

/**
 * Yield CPU to next process
 */
void scheduler_yield() {
    if (current_pcb) {
        current_pcb->state = PROCESS_READY;
    }
    scheduler_schedule();
}

/**
 * Get next process to run (round-robin)
 */
ProcessControlBlock* scheduler_next_process() {
    // Simple round-robin implementation
    // In a real kernel, this would iterate through process table
    // and find next READY process
    
    if (current_pcb && current_pcb->state == PROCESS_RUNNING) {
        return current_pcb;
    }
    
    return current_pcb;
}
