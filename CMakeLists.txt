cmake_minimum_required(VERSION 3.20)
project(bizzy-fusion-os VERSION 1.0.0 LANGUAGES C ASM)

# Project description
set(PROJECT_DESCRIPTION "Bizzy-Fusion OS — A revolutionary operating system combining macOS smoothness, Windows/Android app compatibility, and enterprise-grade security")

# Set C Standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compiler flags for cross-compilation (i686-elf)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffreestanding -O2 -Wall -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostdlib -nostartfiles -node.24faultlibs")

# ASM flags
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -x assembler-with-cpp")

# Define output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Kernel source files
set(KERNEL_SOURCES
    kernel/kernel.C
    kernel/boot.S
    kernel/process/process.c
    kernel/process/scheduler.c
    kernel/memory/paging.c
    kernel/memory/malloc.c
    kernel/interrupts/interrupts.c
    kernel/interrupts/isr.s
    kernel/io/io.c
    kernel/io/ports.c
    kernel/drivers/keyboard.c
    kernel/drivers/timer.c
    kernel/fs/vfs.c
    kernel/fs/inode.c
    utilities/string.c
    utilities/math.c
)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/core
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/process
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/memory
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/interrupts
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/io
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/drivers
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/fs
    ${CMAKE_CURRENT_SOURCE_DIR}/utilities
)

# Linker script
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/kernel/core/linker.ld)

# Create kernel executable
add_executable(bizzy-fusion-os.elf ${KERNEL_SOURCES})

# Link with linker script
set_target_properties(bizzy-fusion-os.elf PROPERTIES
    LINK_FLAGS "-T ${LINKER_SCRIPT} -nostdlib"
)

# Enable testing
enable_testing()

# Test configuration (if test files exist)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    add_subdirectory(tests)
endif()

# Build multiboot2 ISO image (if grub-mkrescue is available)
find_program(GRUB_MKRESCUE grub-mkrescue)
if(GRUB_MKRESCUE)
    add_custom_command(
        TARGET bizzy-fusion-os.elf POST_BUILD
        COMMAND mkdir -p ${CMAKE_BINARY_DIR}/isodir/boot/grub
        COMMAND cp ${CMAKE_BINARY_DIR}/bin/bizzy-fusion-os.elf ${CMAKE_BINARY_DIR}/isodir/boot/
        COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/grub.cfg ${CMAKE_BINARY_DIR}/isodir/boot/grub/ 2>/dev/null || true
        COMMAND ${GRUB_MKRESCUE} -o ${CMAKE_BINARY_DIR}/bin/bizzy-fusion-os.iso ${CMAKE_BINARY_DIR}/isodir/
        COMMENT "Building multiboot2 ISO image"
    )
endif()

# QEMU emulator support
find_program(QEMU qemu-system-i386)
if(QEMU)
    add_custom_target(run
        COMMAND ${QEMU} -cdrom ${CMAKE_BINARY_DIR}/bin/bizzy-fusion-os.iso
        DEPENDS bizzy-fusion-os.elf
        COMMENT "Running Bizzy-Fusion OS in QEMU"
    )

    add_custom_target(debug
        COMMAND ${QEMU} -cdrom ${CMAKE_BINARY_DIR}/bin/bizzy-fusion-os.iso -s -S
        DEPENDS bizzy-fusion-os.elf
        COMMENT "Running Bizzy-Fusion OS in QEMU with GDB support (-s -S)"
    )
endif()

# Print configuration summary
message(STATUS "========================================")
message(STATUS "Bizzy-Fusion OS Build Configuration")
message(STATUS "========================================")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C Flags: ${CMAKE_C_FLAGS}")
message(STATUS "Kernel: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bizzy-fusion-os.elf")
if(GRUB_MKRESCUE)
    message(STATUS "ISO Image: ${CMAKE_BINARY_DIR}/bin/bizzy-fusion-os.iso")
else()
    message(STATUS "ISO Image: Not available (grub-mkrescue not found)")
endif()
message(STATUS "========================================")
