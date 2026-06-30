/**
 * Bizzy-Fusion OS - Scheduler Header
 * 
 * Process scheduling with round-robin algorithm
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include <stdint.h>

#define SCHEDULER_QUANTUM_MS 20

// Scheduler functions
void scheduler_init();
void scheduler_start();
void scheduler_schedule();
void scheduler_yield();
ProcessControlBlock* scheduler_next_process();

#endif // SCHEDULER_H
