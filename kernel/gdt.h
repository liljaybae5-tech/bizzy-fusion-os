# Makefile for Bizzy-Fusion-OS (x86_64, multiboot2)

KDIR := kernel
ISO_DIR := iso
ISO := bizzy-fusion-os.iso
QEMU := qemu-system-x86_64

CC := gcc
AS := as --64
LD := ld -m elf_x86_64

CFLAGS := -m64 -ffreestanding -O2 -Wall -Wextra -fno-builtin -nostdlib -I$(KDIR) -std=gnu11
LDFLAGS := -T $(KDIR)/linker.ld -nostdlib

.PHONY: all iso run clean

all: $(ISO)

# adjust these names if your assembly file is named boot.S/start.S/etc.
$(KDIR)/start.o: $(KDIR)/start.S
	$(AS) -o $@ $<

$(KDIR)/kernel.o: $(KDIR)/kernel.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(KDIR)/kernel.bin: $(KDIR)/start.o $(KDIR)/kernel.o $(KDIR)/linker.ld
	$(LD) $(LDFLAGS) -o $@ $(KDIR)/start.o $(KDIR)/kernel.o

iso: $(KDIR)/kernel.bin
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KDIR)/kernel.bin $(ISO_DIR)/boot/kernel.bin
	cp $(KDIR)/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	# Create ISO with GRUB (try grub-mkrescue then fallback to xorriso)
	grub-mkrescue -o $(ISO) $(ISO_DIR) 2>/dev/null || \
	 xorriso -as mkisofs -R -J -c boot.cat -b boot/grub/i386-pc/eltorito.img \
	  -no-emul-boot -boot-load-size 4 -boot-info-table -o $(ISO) $(ISO_DIR)

$(ISO): iso
	@echo "Created $(ISO)"

run: $(ISO)
	$(QEMU) -cdrom $(ISO) -m 512M -serial stdio

clean:
	rm -rf $(KDIR)/*.o $(KDIR)/kernel.bin $(ISO_DIR)/boot/* $(ISO)
