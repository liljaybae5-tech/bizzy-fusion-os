# Bizzy-Fusion Operating System

## Overview

Bizzy-Fusion-OS is a lightweight, modular operating system kernel designed for educational purposes and embedded systems. It implements core OS concepts including process management, memory management, interrupt handling, and filesystem operations.

## Architecture

### Core Components

```
bizzy-fusion-os/
├── kernel/
│   ├── core/              # Core kernel functionality
│   │   ├── kernel.c       # Main kernel entry point
│   │   ├── kernel.h
│   │   ├── boot.s         # Assembly bootloader
│   │   └── linker.ld      # Linker script
│   ├── process/           # Process and task management
│   │   ├── process.c
│   │   ├── process.h
│   │   ├── scheduler.c
│   │   └── scheduler.h
│   ├── memory/            # Memory management
│   │   ├── paging.c
│   │   ├── paging.h
│   │   ├── malloc.c
│   │   └── malloc.h
│   ├── interrupts/        # Interrupt handling
│   │   ├── interrupts.c
│   │   ├── interrupts.h
│   │   ├── isr.s          # Interrupt service routines
│   │   └── idt.h          # Interrupt descriptor table
│   ├── io/                # I/O operations
│   │   ├── io.c
│   │   ├── io.h
│   │   ├── ports.c
│   │   └── ports.h
│   └── drivers/           # Device drivers
│       ├── keyboard.c
│       ├── keyboard.h
│       ├── timer.c
│       └── timer.h
├── utilities/             # System utilities
│   ├── string.c
│   ├── string.h
│   ├── math.c
│   └── math.h
├── fs/                    # Filesystem
│   ├── vfs.c
│   ├── vfs.h
│   ├── inode.c
│   └── inode.h
├── build/                 # Build output
├── docs/                  # Documentation
├── Makefile               # Build configuration
└── README.md              # This file
```

## Key Features

### 1. Process Management
- Process creation and termination
- Context switching
- Process scheduling (Round-robin)
- Process states (Ready, Running, Waiting, Terminated)

### 2. Memory Management
- Virtual memory with paging
- Physical memory allocation
- Page tables
- Memory protection

### 3. Interrupt Handling
- Interrupt descriptor table (IDT)
- Interrupt service routines (ISRs)
- Interrupt priority levels
- Exception handling

### 4. I/O System
- Port I/O operations
- Device driver framework
- Keyboard input handling
- Timer management

### 5. Filesystem
- Virtual filesystem interface
- Inode-based structure
- File operations (open, read, write, close)
- Directory support

## Building the OS

### Requirements
- GCC (cross-compiler for i686-elf)
- GNU Make
- NASM or AS (assembler)
- QEMU (for emulation)
- LD (linker)

### Build Steps

```bash
# Navigate to the kernel directory
cd kernel

# Build the kernel
make clean
make

# Run in QEMU emulator
make run

# Debug with GDB
make debug
```

## System Specifications

### Target Platform
- **Architecture**: i686 (32-bit x86)
- **Memory**: 64MB RAM (configurable)
- **Boot**: Multiboot compatible bootloader

### Process Management
- **Max Processes**: 256
- **Stack Size per Process**: 4KB
- **Quantum**: 20ms (context switch interval)

### Memory Layout
```
0x00000000 - 0x000FFFFF : Reserved (BIOS, etc)
0x00100000 - 0x00400000 : Kernel code and data
0x00400000 - 0x10000000 : User space (heap and stack)
0x10000000 - 0xFFFFFFFF : Virtual memory (paging)
```

## Boot Process

1. **Bootloader** - GRUB loads kernel at 0x100000
2. **Assembly Setup** - boot.s initializes GDT, stack, paging
3. **Kernel Main** - kernel.c initializes subsystems
4. **Interrupt Setup** - IDT and ISR initialization
5. **Device Drivers** - Keyboard, timer, console
6. **Scheduler Start** - Begin process scheduling
7. **Init Process** - Launch PID 1 (init)

## System Calls

The kernel provides the following system calls to user programs:

```c
// Process management
int fork();                    // Create new process
int exec(char *path);        // Execute program
void exit(int status);       // Terminate process
int wait(int *status);       // Wait for child
int getpid();                // Get process ID

// Memory management
void *malloc(size_t size);   // Allocate memory
void free(void *ptr);        // Deallocate memory
void *mmap(void *addr, ...); // Memory mapping

// File I/O
int open(char *path, int flags);
int close(int fd);
int read(int fd, void *buf, size_t count);
int write(int fd, void *buf, size_t count);

// I/O control
int ioctl(int fd, int request, void *arg);
void sleep(unsigned int ms);
```

## Development Roadmap

- [x] Basic kernel structure
- [x] Boot sequence documentation
- [ ] GDT (Global Descriptor Table) setup
- [ ] IDT and exception handling
- [ ] Paging and virtual memory
- [ ] Process management and scheduling
- [ ] Interrupt handlers
- [ ] Device drivers (keyboard, timer)
- [ ] Filesystem implementation
- [ ] Shell/command interpreter
- [ ] System utilities
- [ ] Network stack

## References

- **OSDev Wiki**: https://wiki.osdev.org
- **Intel x86 Manuals**: https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html
- **Multiboot Specification**: https://www.gnu.org/software/grub/manual/multiboot/
- **POSIX Standards**: https://pubs.opengroup.org/onlinepubs/9699919799/

## License

MIT License - See LICENSE file for details

## Contributing

Contributions are welcome! Please see CONTRIBUTING.md for guidelines.

---

**Bizzy-Fusion-OS**: Building the future, one interrupt at a time! 🚀
