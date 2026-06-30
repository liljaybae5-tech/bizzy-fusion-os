/**
 * Bizzy-Fusion OS - String Utilities Header
 */

#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdint.h>

// String functions
int strlen(const char *str);
void strcpy(char *dest, const char *src);
int strcmp(const char *s1, const char *s2);
char *strcat(char *dest, const char *src);
void *memset(void *s, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);

// Number conversion
void itoa(int num, char *str, int base);
int atoi(const char *str);

#endif // STRING_H
