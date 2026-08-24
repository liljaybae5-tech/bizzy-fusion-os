Bizzy-Fusion-OS — minimal 64-bit hobby kernel (ELF64)
=====================================================

What this is
- A minimal 64-bit (x86_64) hobby kernel scaffold that builds into an ELF64
  binary with a small multiboot2 header so GRUB can load it. The kernel prints
  a message to the VGA text buffer and halts.

Dependencies (example for Debian/Ubuntu)
- build-essential, gcc, binutils, grub-mkrescue, xorriso, qemu-system-x86_64
  sudo apt install build-essential gcc binutils grub-pc-bin xorriso qemu-system-x86_64

Build and run
- make
- make run

Notes
- This kernel expects to be loaded by GRUB (multiboot2). The boot.S includes a
  minimal multiboot2 header. GRUB2 will recognize the header and load the ELF64
  binary into memory; in typical setups GRUB runs in long mode and will transfer
  control to the ELF64 entrypoint.

- If GRUB on your system does not switch to long mode for the kernel, you can
  add a small 32-bit-to-64-bit bootstrap stage that enables long mode. If you
  want that fully self-contained long-mode switcher (builds page tables and
  enables LME), tell me and I'll add it to the repo. (I can add an optional
  boot32.S that performs the switch.)

Next improvements I can make for you
- Add a self-contained 32-bit->64-bit switching stage (build page tables and
  explicitly enable long mode) so the kernel doesn't rely on GRUB's mode.
- Add keyboard input, timer interrupts, and a simple console driver.
- Replace the C kernel with a Rust no_std kernel and a cargo workspace.

Branch
- Files have been committed to the hobby-kernel-64bit branch as requested.
