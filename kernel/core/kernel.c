/**
 * Bizzy-Fusion OS - Stage 0 Kernel
 *
 * Purpose:
 *   Establish a minimal, observable boot path.
 *
 * Boot sequence:
 *   GRUB Multiboot2
 *       -> start
 *       -> kernel_main
 *       -> serial initialization
 *       -> VGA initialization
 *       -> boot diagnostics
 *       -> halt
 */

#include "core/kernel.h"
#include "io/io.h"

#define COM1 0x3F8

static void serial_init(void)
{
    /*
     * Disable interrupts for COM1.
     */
    outb(COM1 + 1, 0x00);

    /*
     * Enable DLAB and set divisor to 3 (38400 baud).
     */
    outb(COM1 + 3, 0x80);
    outb(COM1 + 0, 0x03);
    outb(COM1 + 1, 0x00);

    /*
     * 8 data bits, no parity, one stop bit.
     */
    outb(COM1 + 3, 0x03);

    /*
     * Enable FIFO, clear buffers.
     */
    outb(COM1 + 2, 0xC7);

    /*
     * Enable RTS/DSR.
     */
    outb(COM1 + 4, 0x0B);
}

static int serial_ready(void)
{
    return (inb(COM1 + 5) & 0x20) != 0;
}

static void serial_write_char(char c)
{
    while (!serial_ready()) {
    }

    outb(COM1, (unsigned char)c);
}

static void serial_write(const char *s)
{
    if (s == 0) {
        return;
    }

    while (*s != '\0') {
        if (*s == '\n') {
            serial_write_char('\r');
        }

        serial_write_char(*s);
        ++s;
    }
}

void kernel_main(void)
{
    disable_interrupts();

    /*
     * Initialize serial FIRST.
     * This gives us a machine-readable boot trace even
     * when VGA output is hidden.
     */
    serial_init();

    serial_write("\n");
    serial_write("========================================\n");
    serial_write("       BIZZY-FUSION OPERATING SYSTEM\n");
    serial_write("                 v1.0\n");
    serial_write("========================================\n");

    serial_write("[BOOT] Multiboot2 entry: OK\n");
    serial_write("[BOOT] Kernel entry: OK\n");
    serial_write("[BOOT] Architecture: i386\n");
    serial_write("[BOOT] Serial COM1: OK\n");

    /*
     * Initialize VGA after serial is confirmed.
     */
    clear_screen();

    kprintf("\n");
    kprintf("========================================\n");
    kprintf("       BIZZY-FUSION OPERATING SYSTEM\n");
    kprintf("                 v1.0\n");
    kprintf("========================================\n\n");

    kprintf("[BOOT] Multiboot2 entry: OK\n");
    kprintf("[BOOT] Kernel entry: OK\n");
    kprintf("[BOOT] Architecture: i386\n");
    kprintf("[BOOT] Kernel address: 0x100000\n");
    kprintf("[BOOT] VGA console: OK\n");
    kprintf("[BOOT] Interrupts: DISABLED\n");

    serial_write("[BOOT] VGA initialization complete\n");
    serial_write("[BOOT] Stage 0 initialization complete\n");
    serial_write("[BOOT] Bizzy-Fusion kernel is alive\n");
    serial_write("[BOOT] Entering halt loop\n");

    kprintf("[BOOT] Stage 0 initialization complete.\n");
    kprintf("[BOOT] Bizzy-Fusion kernel is alive.\n");
    kprintf("\n");

    for (;;) {
        halt_cpu();
    }
}

void kernel_panic(
    const char *message,
    const char *file,
    int line
)
{
    disable_interrupts();

    serial_write("\n[PANIC] Bizzy-Fusion kernel panic\n");

    if (message != 0) {
        serial_write("[PANIC] Message: ");
        serial_write(message);
        serial_write("\n");
    }

    clear_screen();

    kprintf("\n");
    kprintf("========================================\n");
    kprintf("            BIZZY-FUSION PANIC\n");
    kprintf("========================================\n\n");

    kprintf("Message: %s\n",
            message != 0 ? message : "(null)");

    kprintf("Location: %s:%d\n",
            file != 0 ? file : "(unknown)",
            line);

    kprintf("\nSystem halted.\n");

    for (;;) {
        halt_cpu();
    }
}
