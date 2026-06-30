/**
 * Bizzy-Fusion OS - Memory Management Header
 * 
 * Virtual memory and paging management
 */

#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>

// Page size (4KB)
#define PAGE_SIZE 4096
#define PAGE_MASK 0xFFFFF000

// Page table entry flags
#define PAGE_PRESENT    0x1
#define PAGE_WRITABLE   0x2
#define PAGE_USER       0x4
#define PAGE_WRITE_THROUGH 0x8
#define PAGE_CACHE_DISABLE 0x10
#define PAGE_ACCESSED   0x20
#define PAGE_DIRTY      0x40

typedef struct {
    uint32_t entries[1024];
} PageTable;

typedef struct {
    uint32_t tables[1024];
} PageDirectory;

// Paging functions
void paging_init();
PageDirectory* paging_create_directory();
PageTable* paging_create_table();
void paging_map_page(PageDirectory *dir, uint32_t vaddr, uint32_t paddr, uint32_t flags);
void paging_unmap_page(PageDirectory *dir, uint32_t vaddr);
void paging_enable();

#endif // PAGING_H
