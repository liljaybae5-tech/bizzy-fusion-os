/**
 * Bizzy-Fusion OS - String Utilities Implementation
 */

#include "string.h"

/**
 * Get string length
 */
int strlen(const char *str) {
    int len = 0;
    while (*str++) len++;
    return len;
}

/**
 * Copy string
 */
void strcpy(char *dest, const char *src) {
    while ((*dest++ = *src++));
}

/**
 * Compare strings
 */
int strcmp(const char *s1, const char *s2) {
    while (*s1 == *s2++) {
        if (*s1++ == '\0') return 0;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)--s2;
}

/**
 * Concatenate strings
 */
char *strcat(char *dest, const char *src) {
    strcpy(dest + strlen(dest), src);
    return dest;
}

/**
 * Set memory
 */
void *memset(void *s, int c, size_t n) {
    unsigned char *p = (unsigned char *)s;
    while (n-- > 0) {
        *p++ = (unsigned char)c;
    }
    return s;
}

/**
 * Copy memory
 */
void *memcpy(void *dest, const void *src, size_t n) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    while (n-- > 0) {
        *d++ = *s++;
    }
    return dest;
}

/**
 * Compare memory
 */
int memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;
    while (n-- > 0) {
        if (*p1 != *p2) return *p1 - *p2;
        p1++;
        p2++;
    }
    return 0;
}

/**
 * Integer to ASCII
 */
void itoa(int num, char *str, int base) {
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    int neg = num < 0;
    if (neg) num = -num;
    
    char buf[32];
    int i = 0;
    
    while (num > 0) {
        int digit = num % base;
        buf[i++] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
        num /= base;
    }
    
    if (neg) buf[i++] = '-';
    
    // Reverse
    int j = 0;
    while (i > 0) {
        str[j++] = buf[--i];
    }
    str[j] = '\0';
}

/**
 * ASCII to integer
 */
int atoi(const char *str) {
    int result = 0;
    int sign = 1;
    
    while (*str == ' ') str++;
    if (*str == '-') sign = -1;
    if (*str == '-' || *str == '+') str++;
    
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    
    return result * sign;
}
