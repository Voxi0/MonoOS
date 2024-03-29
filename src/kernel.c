// Limine
#include<limine/limine.h>

// Freestanding Headers
#include<stddef.h>
#include<stdbool.h>

// Custom Headers
#include"drivers/include/video.h"

// Set Limine Base Revision Version Number
static volatile LIMINE_BASE_REVISION(1);

// Limine Requests
static volatile struct limine_framebuffer_request fbRequest = {
	.id = LIMINE_FRAMEBUFFER_REQUEST,
	.revision = 0
};
static volatile struct limine_bootloader_info_request bootloaderInfoRequest = {
	.id = LIMINE_BOOTLOADER_INFO_REQUEST,
	.revision = 0
};
static volatile struct limine_memmap_request memoryMapRequest = {
	.id = LIMINE_MEMMAP_REQUEST,
	.revision = 0
};

// Disable System Interrupts and Halt System
static void hang(void) {
	asm("cli");
	for(;;) {
		asm("hlt");
	}
}

// Main
void kmain(void) {
	// Ensure That The Limine Base Revision Version Number That We Set Earlier is Actually Supported
	if(LIMINE_BASE_REVISION_SUPPORTED == false) {
		// Halt System
		hang();
	}

	// Ensure That We Have A Framebuffer
	if(fbRequest.response == NULL || fbRequest.response->framebuffer_count < 1) {
		// Halt System
		hang();
	}

	// Fetch The First Available Framebuffer
	volatile struct limine_framebuffer* framebuffer = fbRequest.response->framebuffers[0];

	// Initialize Graphics, Clear Screen and Set Text Color
	initGFX(framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch);
	resetScr(rgbToHex(50, 100, 100));
	setTextColor(rgbToHex(0, 255, 0));

	// Print Bootloader Info
	printf("Bootloader Info:\n");
	printf("\tName: %s\n", bootloaderInfoRequest.response->name);
	printf("\tVersion: %s\n", bootloaderInfoRequest.response->version);
	printf("\tRevision: %lu\n\n", bootloaderInfoRequest.response->revision);

	// Print Memory Map Info
	printf("Memory Map Info:\n");
	printf("\tEntry Count: %lu\n", memoryMapRequest.response->entry_count);
	printf("\tRevision: %lu\n\n", memoryMapRequest.response->revision);

	// Print Framebuffer Info
	printf("Framebuffer Info:\n");
	printf("\tTotal Available: %lu\n", fbRequest.response->framebuffer_count);
	printf("\tWidth: %lu\n", framebuffer->width);
	printf("\tHeight: %lu\n", framebuffer->height);
	printf("\tPitch: %lu\n", framebuffer->pitch);
	printf("\tBPP: %hu\n\n", framebuffer->bpp);

	// Welcome Message
	printf("Welcome to MonoOS!\n");

	// Halt System
	hang();
}
