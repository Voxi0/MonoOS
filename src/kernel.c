// Standard Freestanding Headers
#include<stddef.h>
#include<stdint.h>
#include<stdbool.h>

// Limine
#include<limine.h>

// Custom Headers
#include"drivers/video/video.h"

// Set Limine Base Revision Version
static volatile LIMINE_BASE_REVISION(1);

// Limine Requests
static volatile struct limine_framebuffer_request limineFbRequest = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

// Halt and Disable System Interrupts
static void hang(void) {
    asm("cli");
    for(;;) {
        asm("hlt");
    }
}

// Kernel Entry Point - Main Function
void main(void) {
    // Check if The Limine Base Revision Version We Set Earlier is Actually Supported
    if(LIMINE_BASE_REVISION_SUPPORTED == false) {
        // Disable System Interrupts and Halt System
        hang();
    }

    // Ensure That We Have A Framebuffer
    if(limineFbRequest.response == NULL || limineFbRequest.response->framebuffer_count < 1) {
        // Disable System Interrupts and Halt System
        hang();
    }

    // Fetch The First Available Framebuffer
    struct limine_framebuffer* framebuffer = limineFbRequest.response->framebuffers[0];

    // Initialize Graphics
    initGFX((uint32_t*)framebuffer->address, (uint64_t)framebuffer->width, (uint64_t)framebuffer->height, (uint64_t)framebuffer->pitch);
    resetScreen(0xBAADC);

    // Draw A Circle
    drawCircle(framebuffer->width / 2, framebuffer->height / 2, 80, rgbToHex(100, 200, 0));

    // Disable System Interrupts and Halt System
    hang();
}
