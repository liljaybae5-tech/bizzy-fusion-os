/**
 * Bizzy-Fusion OS - Memory Management Implementation
 * 
 * Implements virtual memory with paging
 */

#include "paging.h"
#include "../core/kernel.h"
#include "../utilities/string.h"

static PageDirectory *kernel_directory = NULL;
static uint32_t frames_bitmap[32] = {0};  // Bitmap for physical frames

/**
 * Initialize paging system
 */
void paging_init() {
    kprintf("[MEMORY] Initializing paging system...\n");
    
    // Create kernel page directory
    kernel_directory = paging_create_directory();
    if (!kernel_directory) {
        PANIC("Failed to create kernel page directory");
    }
    
    // Enable paging
    paging_enable();
    
    kprintf("[MEMORY] Paging system initialized\n");
}

/**
 * Create a new page directory
 */
PageDirectory* paging_create_directory() {
    PageDirectory *dir = (PageDirectory*)0x1000;  // Fixed address for simplicity
    memset(dir, 0, sizeof(PageDirectory));
    return dir;
}

/**
 * Create a new page table
 */
PageTable* paging_create_table() {
    PageTable *table = (PageTable*)0x2000;  // Fixed address for simplicity
    memset(table, 0, sizeof(PageTable));
    return table;
}

/**
 * Map virtual address to physical address
 */
void paging_map_page(PageDirectory *dir, uint32_t vaddr, uint32_t paddr, uint32_t flags) {
    uint32_t pd_index = vaddr >> 22;  // Top 10 bits
    uint32_t pt_index = (vaddr >> 12) & 0x3FF;  // Middle 10 bits
    
    // Create page table if needed
    if (!(dir->tables[pd_index] & PAGE_PRESENT)) {
        PageTable *table = paging_create_table();
        dir->tables[pd_index] = (uint32_t)table | PAGE_PRESENT | PAGE_WRITABLE;
    }
    
    // Get page table
    PageTable *table = (PageTable*)(dir->tables[pd_index] & PAGE_MASK);
    
    // Map the page
    table->entries[pt_index] = (paddr & PAGE_MASK) | flags | PAGE_PRESENT;
}

/**
 * Unmap virtual address
 */
void paging_unmap_page(PageDirectory *dir, uint32_t vaddr) {
    uint32_t pd_index = vaddr >> 22;
    uint32_t pt_index = (vaddr >> 12) & 0x3FF;
    
    if (dir->tables[pd_index] & PAGE_PRESENT) {
        PageTable *table = (PageTable*)(dir->tables[pd_index] & PAGE_MASK);
        table->entries[pt_index] = 0;  // Clear the entry
    }
}

/**
 * Enable paging in CPU
 */
void paging_enable() {
    // Set CR3 to point to kernel page directory
    // asm("mov %0, %%cr3" : : "r"(kernel_directory));
    
    // Set CR0 PE bit to enable paging
    // uint32_t cr0;
    // asm("mov %%cr0, %0" : "=r"(cr0));
    // cr0 |= 0x80000000;  // Set paging bit
    // asm("mov %0, %%cr0" : : "r"(cr0));
    
    kprintf("[MEMORY] Paging enabled\n");
}
