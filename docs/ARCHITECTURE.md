# Bizzy-Fusion-OS Architecture

This document describes the high-level architecture of Bizzy-Fusion-OS and how its components interact.

## System Architecture Overview

```
┌─────────────────────────────────────────────────────┐
│         User Applications & Utilities               │
│  (Shell, Text Editor, File Manager, etc.)           │
└─────────────────────────────────────────────────────┘
                        ▲
                        │ System Calls
                        ▼
┌─────────────────────────────────────────────────────┐
│              System Services Layer                  │
│  (IPC, Signals, Logging, Resource Management)      │
└─────────────────────────────────────────────────────┘
                        ▲
                        │ Internal APIs
                        ▼
┌─────────────────────────────────────────────────────┐
│              Kernel Core Layer                      │
│  (Process Manager, Memory Manager, Filesystem)     │
└─────────────────────────────────────────────────────┘
                        ▲
                        │ HAL Interface
                        ▼
┌─────────────────────────────────────────────────────┐
│        Hardware Abstraction Layer (HAL)             │
│  (Device drivers, Interrupt handling, I/O ops)     │
└─────────────────────────────────────────────────────┘
                        ▲
                        │ Direct Hardware Access
                        ▼
┌─────────────────────────────────────────────────────┐
│              Hardware & CPU                         │
│  (Memory, Disk, Network, Display, etc.)             │
└─────────────────────────────────────────────────────┘
```

## Directory Structure

```
bizzy-fusion-os/
├── kernel/                 # Core kernel implementation
│   ├── arch/              # Architecture-specific code (x86, x64, ARM)
│   ├── core/              # Core kernel subsystems
│   │   ├── process.c      # Process/thread management
│   │   ├── memory.c       # Memory management
│   │   ├── interrupt.c    # Interrupt handling
│   │   └── scheduler.c    # Task scheduling
│   ├── fs/                # Filesystem implementation
│   │   ├── vfs.c          # Virtual filesystem layer
│   │   ├── inode.c        # Inode management
│   │   └── buffer.c       # I/O buffering
│   └── include/           # Kernel headers
│       └── kernel.h       # Main kernel API
│
├── drivers/               # Hardware drivers
│   ├── hal/               # Hardware Abstraction Layer
│   │   ├── device.c       # Device management
│   │   ├── interrupt.c    # Interrupt controller
│   │   └── io.c           # I/O operations
│   ├── disk/              # Disk/storage drivers
│   ├── network/           # Network drivers
│   ├── console/           # Console/framebuffer drivers
│   └── include/           # Driver headers
│
├── lib/                   # System libraries
│   ├── libc/              # C standard library
│   ├── libsys/            # System library (syscall wrappers)
│   └── include/           # Library headers
│
├── utilities/             # System utilities
│   ├── shell/             # Command shell
│   ├── tools/             # Command-line tools (ls, cat, etc.)
│   └── init/              # System initialization
│
├── ui/                    # User interface
│   ├── window/            # Window manager
│   ├── widgets/           # UI components (buttons, text, etc.)
│   ├── graphics/          # Graphics rendering
│   └── apps/              # Built-in applications
│
├��─ tests/                 # Test suite
│   ├── unit/              # Unit tests
│   ├── integration/       # Integration tests
│   └── harness/           # Test framework
│
├── docs/                  # Documentation
│   ├── ARCHITECTURE.md    # This file
│   ├── CODING_STANDARDS.md
│   ├── API.md
│   └── BUILDING.md
│
├── build/                 # Build artifacts (generated)
├── CMakeLists.txt         # Build configuration
└── README.md              # Project overview
```

## Core Components

### 1. Kernel Core (`kernel/core/`)

#### Process & Thread Management (`process.c`)
- Manages process creation, termination, and lifecycle
- Handles thread scheduling and context switching
- Maintains process control blocks (PCBs) and thread control blocks (TCBs)
- Interfaces: `process_create()`, `process_exit()`, `thread_create()`

#### Memory Management (`memory.c`)
- Physical memory allocation/deallocation (page allocator)
- Virtual memory management (paging, page tables)
- Heap allocation (malloc/free-style)
- Interfaces: `kmalloc()`, `kfree()`, `map_page()`, `unmap_page()`

#### Interrupt Handling (`interrupt.c`)
- Sets up interrupt descriptor table (IDT)
- Handles CPU interrupts and exceptions
- Routes hardware interrupts to drivers
- Interfaces: `register_interrupt_handler()`, `send_interrupt()`

#### Scheduler (`scheduler.c`)
- Implements task scheduling algorithm (round-robin)
- Context switching mechanism
- Priority/priority queue management
- Interfaces: `schedule()`, `yield()`, `set_priority()`

### 2. Hardware Abstraction Layer (`drivers/hal/`)

The HAL provides a unified interface to hardware, allowing the kernel to work across different architectures.

#### Device Management (`device.c`)
- Abstracts device operations (open, close, read, write, ioctl)
- Device registration and discovery
- Maintains device driver list
- Interfaces: `device_open()`, `device_read()`, `device_write()`

#### Interrupt Controller (`interrupt.c`)
- Abstracts interrupt controller (PIC, APIC, GIC)
- Enables/disables interrupts
- Handles interrupt routing
- Interfaces: `irq_enable()`, `irq_disable()`, `route_irq()`

#### I/O Operations (`io.c`)
- Port I/O, memory-mapped I/O abstraction
- DMA operations
- Bus access (PCI enumeration)
- Interfaces: `io_read()`, `io_write()`, `dma_transfer()`

### 3. Drivers (`drivers/`)

#### Disk Drivers
- SATA, IDE, NVMe controller drivers
- Handles read/write operations
- Manages disk I/O queuing
- Implements: `disk_read_sector()`, `disk_write_sector()`

#### Network Drivers
- Ethernet NIC drivers
- Packet transmission/reception
- Link management
- Implements: `net_send_packet()`, `net_receive_packet()`

#### Console Driver
- Framebuffer/display management
- Text rendering
- Input handling
- Implements: `console_putchar()`, `console_clear()`, `console_get_input()`

### 4. Filesystem (`kernel/fs/`)

#### Virtual Filesystem Layer (`vfs.c`)
- Abstracts different filesystem types
- Provides unified file operations
- Manages mount points
- Interfaces: `vfs_open()`, `vfs_read()`, `vfs_write()`, `vfs_mount()`

#### Inode Management (`inode.c`)
- Represents files/directories in memory
- Maintains file metadata (permissions, size, timestamps)
- Inode cache/hash table
- Interfaces: `inode_create()`, `inode_lookup()`, `inode_write()`

#### I/O Buffering (`buffer.c`)
- Page cache for disk reads/writes
- Reduces I/O operations
- Write-back caching
- Interfaces: `buffer_read()`, `buffer_write()`, `buffer_flush()`

### 5. System Libraries (`lib/`)

#### C Standard Library (`libc/`)
- Standard C functions (string, memory, math)
- POSIX compatibility layer
- Standard file descriptors (stdin, stdout, stderr)

#### System Library (`libsys/`)
- System call wrappers
- Process/memory management APIs
- IPC primitives
- Interfaces: `fork()`, `exec()`, `wait()`, `mmap()`, `pipe()`

### 6. User Interface (`ui/`)

#### Window Manager
- Manages windows and event routing
- Window lifecycle (create, maximize, minimize, close)
- Z-order management
- Interfaces: `create_window()`, `dispatch_event()`

#### Graphics Rendering
- Drawing primitives (lines, rectangles, circles)
- Text rendering
- Double buffering
- Interfaces: `draw_line()`, `draw_text()`, `render_frame()`

#### Widget System
- Reusable UI components
- Button, text input, menu, scroll bar
- Event handling and callbacks
- Interfaces: `create_button()`, `create_textbox()`, `on_click_handler()`

## Data Flow Examples

### File Read Operation

```
User Application
    │
    └─► read() syscall
        │
        └─► vfs_read() - VFS layer
            │
            ├─► inode_lookup() - Find file inode
            │
            └─► buffer_read() - Read from cache or disk
                │
                └─► disk driver I/O
                    │
                    └─► DMA/HAL interface
                        │
                        └─► Physical disk read
```

### Application Execution

```
User Application (process 1)
    │
    └─► fork() syscall
        │
        └─► process_create() - Create new PCB
            │
            ├─► memory_copy() - Copy memory space
            │
            └─► Return to process 1
    
    └─► exec() syscall
        │
        └─► load_executable()
            │
            ├─► vfs_open() - Open binary file
            ├─► memory_allocate() - Allocate memory for segments
            ├─► Load code/data sections
            └─► scheduler_add() - Add to ready queue
                │
                └─► scheduler runs process 2
```

### Interrupt Handling

```
Hardware generates interrupt
    │
    └─► CPU receives interrupt signal
        │
        └─► interrupt_handler() - Enter kernel
            │
            ├─► Get interrupt number from IDT
            │
            ├─► Save process context
            │
            ├─► Call interrupt service routine (ISR)
            │   └─► Driver/subsystem handles interrupt
            │
            ├─► Restore process context
            │
            └─► Return from interrupt (iret)
```

## Key Interfaces & Conventions

### Syscall Convention (x86-64)

```
User Application → Syscall
  Arguments: rdi, rsi, rdx, r10, r8, r9
  Syscall Number: rax
  Return Value: rax
  Error: Negative rax or errno
```

### Error Handling

- Functions return error codes or status (typically negative values)
- `errno` global variable for syscalls
- NULL pointers for allocation failures
- Error codes defined in `kernel/include/errno.h`

### Locking & Concurrency

- Spinlocks for short critical sections
- Mutexes for longer critical sections
- Read-write locks for reader/writer problems
- Lock hierarchy to prevent deadlocks

## Inter-Component Communication

### Kernel → Driver Communication
- Drivers register handlers with kernel subsystems
- Kernel calls driver functions through function pointers
- Callbacks for event notifications

### User → Kernel Communication
- System calls via `int 0x80` (x86) or `syscall` (x86-64)
- System call dispatcher routes to handlers
- Arguments passed via registers/stack

### Device → Kernel Communication
- Interrupts signal events to kernel
- Interrupt handlers parse device status
- Driver reads device status registers

## Memory Layout (Runtime)

```
Virtual Address Space (per process):
┌─────────────────────────────┐ 0xFFFFFFFF
│    Kernel Space             │
│    (Not accessible)         │
├───────────���─────────────────┤ 0x80000000 (typical)
│    Stack (grows down)       │
│    ↓                        │
│                             │
├─────────────────────────────┤
│    Heap (grows up)          │
│    ↑                        │
├─────────────────────────────┤
│    BSS (uninitialized)      │
├─────────────────────────────┤
│    Data (initialized)       │
├─────────────────────────────┤
│    Text (code)              │
└─────────────────────────────┘ 0x00000000
```

## Extension Points

Developers can extend Bizzy-Fusion-OS in these areas:

1. **New Drivers** - Add in `drivers/` directory following HAL interface
2. **New Filesystems** - Implement VFS interface in `kernel/fs/`
3. **New Syscalls** - Add syscall handler in `kernel/core/`
4. **New Applications** - Create in `utilities/` or `ui/apps/`
5. **Architecture Support** - Add in `kernel/arch/` for new CPU architecture

---

For more information on building, coding standards, and APIs, see the other documentation files in the `docs/` directory.
