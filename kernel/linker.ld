OUTPUT_FORMAT(elf64-x86-64)
ENTRY(start)

/* Place the kernel at 1 MiB (0x00100000) - conventional kernel load area */
SECTIONS
{
  . = 0x00100000;

  .text :
  {
    *(.multiboot2_header)
    *(.text)
  }

  .rodata : { *(.rodata) }
  .data : { *(.data) }

  .bss :
  {
    __bss_start = .;
    *(.bss)
    *(COMMON)
    __bss_end = .;
  }
}
