/**
 * Bizzy-Fusion OS - Keyboard Driver Header
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

// Keyboard buffer size
#define KEYBOARD_BUFFER_SIZE 256

void keyboard_init();
void keyboard_handler(uint32_t num);
char keyboard_getchar();

#endif // KEYBOARD_H
