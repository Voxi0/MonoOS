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
extern uint8_t _binary_assets_fonts_3270_sfn_start;

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
    ssfn_src = (ssfn_font_t*)&_binary_assets_fonts_3270_sfn_start;
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

// Clear Screen and Reset Cursor Position
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

// Render A Character
void drawChar(uint32_t unicode, uint64_t xPos, uint64_t yPos, uint32_t textColor) {
	// Set Cursor Position
	ssfn_dst.x = xPos;
	ssfn_dst.y = yPos;

	// Handle Special Characters
	switch(unicode) {
		// Newline
		case '\n':
			ssfn_dst.y += gfx.glyphHeight;
	}

	// Check if Text Fits The Screen
	if((ssfn_dst.x + gfx.glyphWidth) > (int)gfx.fbWidth) {
		ssfn_dst.x = 0;
		ssfn_dst.y += gfx.glyphHeight;
	}
	if(ssfn_dst.y >= (int)gfx.fbHeight) {
		ssfn_dst.x = 0;
		ssfn_dst.y = gfx.fbHeight - gfx.glyphHeight;
	}

	// Set Text Color and Render Character
	ssfn_dst.fg = textColor;
	ssfn_putc(unicode);
}
void printStr(char* string, uint32_t textColor) {
	while(*string) {
		drawChar(ssfn_utf8(&string), ssfn_dst.x, ssfn_dst.y, textColor);
	}
}
