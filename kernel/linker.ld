OUTPUT_FORMAT(elf32-i386)
ENTRY(kernel_entry)

SECTIONS
{
  /* Load the kernel at 1 MiB (standard multiboot load area) */
  . = 0x00100000;

  .text : {
    /* Include a multiboot header if present */
    *(.multiboot)
    *(.text)
  }

  .rodata : { *(.rodata) }
  .data : { *(.data) }

  .bss : {
    __bss_start = .;
    *(.bss)
    *(COMMON)
    __bss_end = .;
  }
}
