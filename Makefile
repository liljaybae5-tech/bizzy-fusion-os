# Minimal build + ISO + QEMU run for Bizzy-Fusion-OS (64-bit ELF64)
KERNEL_DIR := kernel
ISO_DIR := iso
ISO := bizzy-fusion-os.iso
QEMU := qemu-system-x86_64
CFLAGS := -m64 -ffreestanding -O2 -Wall -Wextra -fno-builtin -nostdlib
AS := as --64
LD := ld -m elf_x86_64

.PHONY: all iso run clean

all: $(ISO)

$(KERNEL_DIR)/boot.o: $(KERNEL_DIR)/boot.S
	$(AS) -o $@ $<

$(KERNEL_DIR)/kernel.o: $(KERNEL_DIR)/kernel.c
	gcc $(CFLAGS) -c -o $@ $<

$(KERNEL_DIR)/kernel.bin: $(KERNEL_DIR)/boot.o $(KERNEL_DIR)/kernel.o $(KERNEL_DIR)/linker.ld
	$(LD) -T $(KERNEL_DIR)/linker.ld -o $@ $(KERNEL_DIR)/boot.o $(KERNEL_DIR)/kernel.o

iso: $(KERNEL_DIR)/kernel.bin
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_DIR)/kernel.bin $(ISO_DIR)/boot/kernel.bin
	cp $(KERNEL_DIR)/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	# Create ISO with GRUB (uses grub-mkrescue)
	grub-mkrescue -o $(ISO) $(ISO_DIR) 2>/dev/null || xorriso -as mkisofs -R -J -c boot.cat -b boot/grub/i386-pc/eltorito.img -no-emul-boot -boot-load-size 4 -boot-info-table -o $(ISO) $(ISO_DIR)

$(ISO): iso
	@echo "Created $(ISO)"

run: $(ISO)
	$(QEMU) -cdrom $(ISO) -m 512M -serial stdio -enable-kvm

clean:
	rm -rf $(KERNEL_DIR)/*.o $(KERNEL_DIR)/kernel.bin $(ISO_DIR)/boot/* $(ISO)
