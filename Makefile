# Variables
OS_NAME=MonoOS

# Run/Emulate OS
run: build/${OS_NAME}.iso
	@qemu-system-x86_64 --enable-kvm -cdrom build/${OS_NAME}.iso

# OS ISO File
build/${OS_NAME}.iso: limine.cfg build/isodir/boot/${OS_NAME}.bin
	@cp -v limine.cfg 3rdparty/limine/limine-bios.sys 3rdparty/limine/limine-bios-cd.bin 3rdparty/limine/limine-uefi-cd.bin build/isodir/boot/
	@cp -v 3rdparty/limine/BOOTIA32.EFI 3rdparty/limine/BOOTX64.EFI build/isodir/EFI/BOOT/
	@xorriso -as mkisofs -b boot/limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot boot/limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        build/isodir -o build/${OS_NAME}.iso

# OS Binary File
build/isodir/boot/${OS_NAME}.bin: build/kernel.o build/video.o build/3270.o linker.ld
	@gcc -T linker.ld build/*.o -o build/isodir/boot/${OS_NAME}.bin -ffreestanding -O2 -nostdlib -lgcc

# Source Files
# Kernel
build/kernel.o: src/kernel.c
	@gcc -I 3rdparty/ -c src/kernel.c -o build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

# Drivers
build/video.o: src/drivers/video/video.c
	@gcc -I 3rdparty/ -c src/drivers/video/video.c -o build/video.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

# Fonts
build/FreeSerif.o: assets/fonts/FreeSerif.sfn
	@ld -r -b binary -o build/FreeSerif.o assets/fonts/FreeSerif.sfn
build/3270.o: assets/fonts/3270.sfn
	@ld -r -b binary -o build/3270.o assets/fonts/3270.sfn
build/symbola.o: assets/fonts/symbola.sfn
	@ld -r -b binary -o build/symbola.o assets/fonts/symbola.sfn
build/unifont.o: assets/fonts/unifont.sfn
	@ld -r -b binary -o build/unifont.o assets/fonts/unifont.sfn

# Clean Everything
clean:
	rm -rf build
	mkdir -p build/isodir/boot
	mkdir -p build/isodir/EFI/BOOT
