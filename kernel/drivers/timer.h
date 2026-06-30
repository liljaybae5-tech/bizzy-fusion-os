/**
 * Bizzy-Fusion OS - Timer Driver Header
 */

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

#define TIMER_FREQUENCY 1000  // 1000 Hz

void timer_init();
void timer_handler(uint32_t num);
uint32_t get_ticks();
void sleep_ms(uint32_t ms);

#endif // TIMER_H
