# Variables
OS_NAME = "MonoOS"
BOOT_DIR = "build/isodir/boot"
EFI_BOOT_DIR = "build/isodir/EFI/BOOT"
C_LINK_FLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Werror -fno-stack-protector

# Run/Emulate OS
run: build/${OS_NAME}.iso
	@qemu-system-x86_64 --enable-kvm -cdrom build/${OS_NAME}.iso

# OS ISO File
build/${OS_NAME}.iso: limine.cfg ${BOOT_DIR}/${OS_NAME}.bin
	@cp -v limine.cfg 3rdparty/limine/limine-bios.sys 3rdparty/limine/limine-bios-cd.bin 3rdparty/limine/limine-uefi-cd.bin ${BOOT_DIR}/limine/
	@cp -v 3rdparty/limine/BOOTIA32.EFI 3rdparty/limine/BOOTX64.EFI ${EFI_BOOT_DIR}/
	@xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot boot/limine/limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        build/isodir -o build/${OS_NAME}.iso

# OS Binary
${BOOT_DIR}/${OS_NAME}.bin: linker.ld build/unifont.o build/kernel.o build/video.o build/printf.o
	@gcc -T linker.ld build/*.o -o ${BOOT_DIR}/${OS_NAME}.bin -ffreestanding -O2 -nostdlib -lgcc
	@rm build/*.o

# Source Files
# Kernel
build/kernel.o: src/kernel.c
	@gcc -I 3rdparty/ -c src/kernel.c -o build/kernel.o ${C_LINK_FLAGS}

# Drivers
build/video.o: src/drivers/video.c
	@gcc -I 3rdparty/ -c src/drivers/video.c -o build/video.o ${C_LINK_FLAGS}

# Dependencies (Libraries)
build/printf.o: 3rdparty/tiny-printf/printf.c
	@gcc -c 3rdparty/tiny-printf/printf.c -o build/printf.o ${C_LINK_FLAGS}

# Fonts
build/unifont.o: assets/fonts/unifont.sfn
	@ld -r -b binary -o build/unifont.o assets/fonts/unifont.sfn

# Clean Everything
clean:
	@rm -rf build
	@mkdir -p ${BOOT_DIR}/limine
	@mkdir -p ${EFI_BOOT_DIR}
