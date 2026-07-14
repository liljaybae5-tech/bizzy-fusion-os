# Bizzy-Fusion-OS Coding Standards

This document outlines the coding standards and best practices for Bizzy-Fusion-OS. All contributors are expected to follow these guidelines to maintain code quality and consistency.

## Language Standards

### C/C++ Code Style

We follow the **Google C++ Style Guide** with some modifications for kernel code.

#### Naming Conventions

**Functions:**
```c
// Snake case for C functions
void process_create(struct process *proc);
void memory_allocate(size_t size);
void* kmalloc(size_t size);
```

**Variables:**
```c
// Snake case for local and static variables
int page_count = 0;
struct process *current_process;
static int interrupt_count = 0;

// ALL_CAPS for constants and macros
#define MAX_PROCESSES 256
#define PAGE_SIZE 4096
const int SCHEDULER_QUANTUM = 10;
```

**Structs & Enums:**
```c
// Pascal case for type names
struct ProcessControlBlock {
    int pid;
    int priority;
};

enum InterruptType {
    IRQ_TIMER = 0,
    IRQ_KEYBOARD = 1,
};

typedef enum InterruptType interrupt_type_t;
```

**Typedefs:**
```c
// Use _t suffix for typedef'd types
typedef unsigned int uint_t;
typedef struct process_t process_t;
typedef void (*interrupt_handler_t)(int);
```

#### Indentation & Formatting

```c
// Use 4 spaces for indentation (NOT tabs)
void function_example(int parameter1, int parameter2)
{
    if (parameter1 > 0) {
        for (int i = 0; i < parameter1; i++) {
            do_something(i);
        }
    }
}

// Opening brace on new line for functions (K&R style variation)
// but same line for control structures
if (condition) {
    // code
}

while (condition) {
    // code
}
```

**Line Length:**
- Maximum 100 characters per line
- If exceeding, break into multiple lines

```c
// Good
void very_long_function_name(int param1, int param2,
                              int param3, int param4)
{
    // implementation
}

// Bad - line too long
void very_long_function_name(int param1, int param2, int param3, int param4) { /* impl */ }
```

#### Comments & Documentation

```c
/**
 * brief_description_here
 *
 * Longer description explaining what this function does, why it exists,
 * and any important implementation details.
 *
 * @param param1 Description of first parameter
 * @param param2 Description of second parameter
 * @return Description of return value
 * 
 * @note Any important notes for users
 * @see Related functions
 */
void well_documented_function(int param1, char *param2)
{
    // Inline comments for complex logic
    // Use // for single-line comments
    
    /* Use */ /* for */ /* multi-line if needed */
    
    // TODO: Items to be completed later
    // FIXME: Known issues that need fixing
    // NOTE: Important clarifications
    // HACK: Temporary solutions needing improvement
}
```

**Documentation Requirements:**
- All public functions MUST have doc comments
- All structs and enums MUST have doc comments
- Document non-obvious logic with inline comments
- Document WHY, not WHAT (code shows what)

### Include Files

```c
// Order of includes:
// 1. Corresponding .h file (if .c file)
// 2. Standard C library headers
// 3. System headers
// 4. Project headers

// Example in kernel/core/process.c:
#include "process.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>

#include "../include/kernel.h"
#include "../../drivers/hal/device.h"
```

**Use include guards in headers:**
```c
#ifndef PROCESS_H
#define PROCESS_H

// declarations

#endif // PROCESS_H
```

## Error Handling

### Return Codes

Functions should return meaningful error codes:

```c
// Define error codes
#define SUCCESS 0
#define ERROR_NO_MEMORY -1
#define ERROR_INVALID_PARAM -2
#define ERROR_NOT_FOUND -3
#define ERROR_PERMISSION_DENIED -13

int process_create(struct process *proc)
{
    if (proc == NULL) {
        return ERROR_INVALID_PARAM;
    }
    
    void *memory = kmalloc(sizeof(struct process));
    if (memory == NULL) {
        return ERROR_NO_MEMORY;
    }
    
    // ... more code
    return SUCCESS;
}
```

### Checking Errors

```c
int result = process_create(&my_process);
if (result != SUCCESS) {
    // Handle error appropriately
    kernel_panic("Failed to create process");
}
```

## Memory Management

### Allocation Patterns

```c
// Always check allocations
struct process *proc = kmalloc(sizeof(struct process));
if (proc == NULL) {
    return ERROR_NO_MEMORY;
}

// Initialize memory
memset(proc, 0, sizeof(struct process));

// Use in function

// Clean up before returning
kfree(proc);
```

### Resource Cleanup

```c
// Use goto for cleanup (acceptable in C for error handling)
int function_with_resources(void)
{
    int *buffer = kmalloc(sizeof(int) * 100);
    if (!buffer) {
        goto error_buffer;
    }
    
    struct device *dev = device_open("/dev/disk0");
    if (!dev) {
        goto error_device;
    }
    
    // Do work
    int result = do_work(buffer, dev);
    
    // Cleanup (reverse order of allocation)
    device_close(dev);
error_device:
    kfree(buffer);
error_buffer:
    return result;
}
```

## Type Safety

### Use Proper Types

```c
// Good - explicit types
uint32_t value = 0xDEADBEEF;
uint8_t byte = 0xFF;
int count = 42;

// Avoid - ambiguous types
int value = 0xDEADBEEF;  // Should be uint32_t
unsigned short byte = 0xFF;  // Should be uint8_t
```

### Struct Initialization

```c
// Good - explicit initialization
struct process proc = {
    .pid = 0,
    .priority = PRIORITY_NORMAL,
    .state = PROCESS_READY,
    .parent = NULL,
};

// Avoid - relying on implicit initialization
struct process proc;
proc.pid = 0;
proc.priority = PRIORITY_NORMAL;
```

## Kernel-Specific Standards

### Critical Sections

```c
// Protect shared data with locks
spinlock_t process_list_lock = SPINLOCK_INIT;

void add_process(struct process *proc)
{
    spinlock_acquire(&process_list_lock);
    
    // Critical section - update shared data
    list_add(&process_list, proc);
    
    spinlock_release(&process_list_lock);
}
```

### Interrupt-Safe Code

```c
// Disable interrupts for very short sections
interrupt_state_t state = interrupt_disable();

// Critical section
shared_variable++;

interrupt_restore(state);
```

### Avoid in Kernel

```c
// NEVER use recursion in kernel code
// - Limited stack space
// - Unpredictable behavior

// NEVER use floating point without special handling
// - FPU state management required
// - Context switch complications

// NEVER use dynamic recursion depth
// - Use iterative algorithms instead
```

## Testing Requirements

### Unit Tests

All new code MUST include unit tests:

```c
// tests/unit/test_memory.c
#include "../test_framework.h"
#include "../../kernel/include/memory.h"

TEST(test_kmalloc_allocation)
{
    void *ptr = kmalloc(100);
    ASSERT_NOT_NULL(ptr);
    kfree(ptr);
}

TEST(test_kmalloc_zero_size)
{
    void *ptr = kmalloc(0);
    ASSERT_NULL(ptr);  // or ASSERT_NOT_NULL depending on design
}

TEST(test_kfree_null)
{
    kfree(NULL);  // Should not crash
    ASSERT_TRUE(1);
}
```

### Test Coverage

- Aim for 80%+ code coverage
- Test error paths, not just happy path
- Test boundary conditions
- Test with invalid inputs

## File Structure

### Header Files (`*.h`)

```c
#ifndef MODULE_NAME_H
#define MODULE_NAME_H

#include <stddef.h>
#include <stdint.h>

/**
 * Description of data structure
 */
struct module_name {
    int field1;
    void *field2;
};

/**
 * Public API - brief description
 *
 * @param param Parameter description
 * @return Return value description
 */
void module_function(int param);

#endif // MODULE_NAME_H
```

### Source Files (`*.c`)

```c
/**
 * module_name.c - Brief module description
 *
 * Longer description of module purpose and responsibility
 */

#include "module_name.h"

#include <stdio.h>

// Private functions (static)
static void private_function(void)
{
    // implementation
}

// Public functions
void module_function(int param)
{
    if (param < 0) {
        return;
    }
    
    private_function();
}
```

## Code Review Checklist

- [ ] Follows naming conventions
- [ ] Uses 4-space indentation
- [ ] Lines under 100 characters
- [ ] Doc comments on public functions
- [ ] Error checking on all allocations
- [ ] Proper error codes returned
- [ ] No memory leaks
- [ ] Thread-safe (locks where needed)
- [ ] Unit tests included (80%+ coverage)
- [ ] No compiler warnings
- [ ] No TODOs without issue tracker links

## Tools & Validation

### Compilation Flags

```bash
# Use these compilation flags to catch issues early
-Wall -Wextra -Werror
-Wconversion -Wsign-conversion
-Wformat=2 -Wstrict-prototypes
-fstack-protector-strong
-O2  # Optimization
```

### Static Analysis

Run `clang-analyzer` or `cppcheck`:
```bash
make analyze
```

### Code Formatting

Use `clang-format` with provided configuration:
```bash
clang-format -i file.c
```

## Git Commit Standards

### Commit Message Format

```
<type>(<scope>): <subject>

<body>

<footer>
```

**Type:**
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation
- `style`: Code formatting
- `refactor`: Code restructuring
- `perf`: Performance improvement
- `test`: Test changes

**Example:**
```
feat(memory): Implement buddy allocator for page allocation

Added buddy allocator algorithm for efficient physical memory
management. Reduces fragmentation compared to naive allocator.

Closes #42
```

## References

- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- [Linux Kernel Coding Style](https://www.kernel.org/doc/html/latest/process/coding-style.html)
- [CERT C Secure Coding](https://wiki.sei.cmu.edu/confluence/display/c)

## Questions?

If you're unsure about any standard:
1. Check existing code for examples
2. Ask in the GitHub issue
3. Open a discussion in the repository
4. Request clarification in code review

Thank you for maintaining high code quality!
