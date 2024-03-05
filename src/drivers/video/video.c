// Freestanding Standard Headers
#include<stddef.h>

// Limine
#include<limine/limine.h>

// SSFN
#define SSFN_NOIMPLEMENTATION
#define SSFN_CONSOLEBITMAP_TRUECOLOR
#include<ssfn.h>

#include"video.h"

// Graphics Struct
struct gfx_struct gfx;

// Font
extern uint64_t _binary_assets_fonts_3270_sfn_start;

// Convert RGB/RGBA to Hex Color Code
uint32_t rgbToHex(int r, int g, int b) {
    return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}
uint32_t rgbaToHex(int r, int g, int b, int a) {
    return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8) + (a & 0xff);
}

// Initialize Video
void initGFX(uint32_t* fbAddress, uint64_t fbWidth, uint64_t fbHeight, uint64_t fbPitch) {
    // Initialize Graphics Struct
    gfx.fbAddress = fbAddress;
    gfx.fbWidth = fbWidth;
    gfx.fbHeight = fbHeight;
    gfx.fbPitch = fbPitch;

    // Initialize SSFN
    ssfn_src = (ssfn_font_t*)_binary_assets_fonts_3270_sfn_start;
    ssfn_dst.ptr = (uint8_t*)fbAddress;
    ssfn_dst.w = fbWidth;
    ssfn_dst.h = fbHeight;
    ssfn_dst.p = fbPitch;
    ssfn_dst.x = ssfn_dst.y = 0;
}

// Draw A Pixel
void drawPixel(int xPos, int yPos, uint32_t color) {
    size_t fbIndex = yPos * (gfx.fbPitch / sizeof(uint32_t)) + xPos;
    uint32_t* fbPtr = (uint32_t*)gfx.fbAddress;
    fbPtr[fbIndex] = color;
}

// Draw A Circle
void drawCircle(int centerX, int centerY, int radius, uint32_t color) {
    for (int y = centerY - radius; y <= centerY + radius; y++) {
        for(int x = centerX - radius; x <= centerX + radius; x++) {
            if((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY) <= radius * radius) {
                drawPixel(x, y, color);
            }
        }
    }
}

// Set Background Color
void resetScreen(const uint32_t bgColor) {
    // Reset SSFN Cursor Position
    ssfn_dst.x = 0;
    ssfn_dst.y = 0;

    // Clear Screen With Color
    ssfn_dst.bg = bgColor;
    for(uint64_t y = 0; y < gfx.fbHeight; y++) {
        for(uint64_t x = 0; x < gfx.fbWidth; x++) {
            drawPixel(x, y, ssfn_dst.bg);
        }
    }
}
