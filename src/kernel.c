// Limine
#include<limine/limine.h>

// Freestanding Headers
#include<stddef.h>
#include<stdbool.h>

// Custom Headers
#include"drivers/include/video.h"

// Set Limine Base Revision Version Number
static volatile LIMINE_BASE_REVISION(1)

// Limine Requests
static volatile struct limine_framebuffer_request fbRequest = {
	.id = LIMINE_FRAMEBUFFER_REQUEST,
	.revision = 0
};

// Disable System Interrupts and Halt System
static void hang(void) {
	__asm__("cli");
	for(;;) {
		__asm__("hlt");
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
