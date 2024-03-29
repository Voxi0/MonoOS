# Phony
.PHONY: run clean

# Language Config
CC := gcc
CFLAGS := -I 3rdparty/ -O2 -Wall -Wextra -Werror -Wpedantic -fno-stack-protector -nostdlib -lgcc

# Variables
OS_NAME := "MonoOS"
BOOT_DIR := "build/isodir/boot"
EFI_BOOT_DIR := "build/isodir/EFI/BOOT"
OBJ_FILES := build/boot.o build/kernel.o build/unifont.o build/video.o build/printf.o

# Run/Emulate OS
run: build/$(OS_NAME).iso
	@qemu-system-x86_64 --enable-kvm -cdrom build/$(OS_NAME).iso

# OS ISO File
build/$(OS_NAME).iso: limine.cfg $(BOOT_DIR)/$(OS_NAME).bin
	@cp -v limine.cfg 3rdparty/limine/limine-bios.sys 3rdparty/limine/limine-bios-cd.bin 3rdparty/limine/limine-uefi-cd.bin $(BOOT_DIR)/limine/
	@cp -v 3rdparty/limine/BOOTIA32.EFI 3rdparty/limine/BOOTX64.EFI $(EFI_BOOT_DIR)/
	@xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot boot/limine/limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        build/isodir -o build/$(OS_NAME).iso

# OS Binary
$(BOOT_DIR)/$(OS_NAME).bin: linker.ld $(OBJ_FILES)
	@$(CC) $(CFLAGS) -T linker.ld build/*.o -o $(BOOT_DIR)/$(OS_NAME).bin
	@rm build/*.o

# Source Files
# Boot
build/boot.o: src/boot.asm
	@nasm -f elf64 src/boot.asm -o build/boot.o

# Kernel
build/kernel.o: src/kernel.c
	@$(CC) $(CFLAGS) -c src/kernel.c -o build/kernel.o

# Drivers
build/video.o: src/drivers/video.c
	@$(CC) $(CFLAGS) -c src/drivers/video.c -o build/video.o

# Dependencies (Libraries)
build/printf.o: 3rdparty/tiny-printf/printf.c
	@$(CC) $(CFLAGS) -c 3rdparty/tiny-printf/printf.c -o build/printf.o

# Fonts
build/unifont.o: assets/fonts/unifont.sfn
	@ld -r -b binary -o build/unifont.o assets/fonts/unifont.sfn

# Clean Everything
clean:
	@rm -rf build
	@mkdir -p $(BOOT_DIR)/limine
	@mkdir -p $(EFI_BOOT_DIR)
