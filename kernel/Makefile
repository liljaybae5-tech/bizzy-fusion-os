# Build configuration for Bizzy-Fusion OS Kernel

# Compiler and tools
CC = gcc
AS = as
LD = ld
ARCH = i686

# Flags
CFLAGS = -std=c99 -ffreestanding -nostdlib -m32 -Wall -Wextra
ASFLAGS = --32
LDFLAGS = -m elf_i386 -T linker.ld

# Source files
SOURCES = core/kernel.c \
          process/process.c \
          process/scheduler.c \
          memory/paging.c \
          interrupts/interrupts.c \
          io/io.c \
          utilities/string.c \
          drivers/timer.c \
          drivers/keyboard.c

OBJECTS = $(SOURCES:.c=.o)
KERNEL_BIN = kernel.elf

# Targets
all: clean $(KERNEL_BIN)

$(KERNEL_BIN): $(OBJECTS)
	@echo "[LINK] Linking kernel..."
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS)
	@echo "[BUILD] Kernel built successfully!"

%.o: %.c
	@echo "[CC] Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

run: $(KERNEL_BIN)
	@echo "[RUN] Starting QEMU..."
	qemu-system-i386 -kernel $(KERNEL_BIN) -m 64M

debug: $(KERNEL_BIN)
	@echo "[DEBUG] Starting GDB debugger..."
	qemu-system-i386 -kernel $(KERNEL_BIN) -m 64M -s -S &
	sleep 1
	i686-elf-gdb $(KERNEL_BIN)

clean:
	@echo "[CLEAN] Removing build artifacts..."
	@rm -f $(OBJECTS) $(KERNEL_BIN)
	@echo "[CLEAN] Done"

.PHONY: all run debug clean
