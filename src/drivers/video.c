#include"include/video.h"

// SSFN
#define SSFN_NOIMPLEMENTATION
#define SSFN_CONSOLEBITMAP_TRUECOLOR
#include<ssfn.h>

// GFX Struct
struct gfxStruct gfxInfo;

// Font
extern char _binary_assets_fonts_unifont_sfn_start;

// Convert RGB/RGBA to Hex Color Codes
uint32_t rgbToHex(int red, int green, int blue) {
	return ((red & 0xFF) << 16) + ((green & 0xFF) << 8) + (blue & 0XFF);
}
uint32_t rgbaToHex(int red, int green, int blue, int alpha) {
	return ((red & 0xFF) << 24) + ((green & 0xFF) << 16) + ((blue & 0xFF) << 8) + (alpha & 0xFF);
}

// Initialize Video
void initGFX(uint32_t* fbAddr, size_t fbWidth, size_t fbHeight, size_t fbPitch) {
	// Initialize GFX Struct
	gfxInfo.fbAddress = fbAddr;
	gfxInfo.fbWidth = fbWidth;
	gfxInfo.fbHeight = fbHeight;
	gfxInfo.fbPitch = fbPitch;
	gfxInfo.glyphWidth = 8;
	gfxInfo.glyphHeight = 16;

	// Initialize SSFN
	ssfn_src = (ssfn_font_t*)&_binary_assets_fonts_unifont_sfn_start;
	ssfn_dst.ptr = (uint8_t*)fbAddr;
	ssfn_dst.w = fbWidth;
	ssfn_dst.h = fbHeight;
	ssfn_dst.p = fbPitch;
	ssfn_dst.x = ssfn_dst.y = 0;
}

// Draw A Pixel
void drawPixel(int xPos, int yPos, uint32_t color) {
	size_t fbIndex = yPos * (gfxInfo.fbPitch / sizeof(uint32_t)) + xPos;
	gfxInfo.fbAddress[fbIndex] = color;
}

// Clear Screen and Reset Cursor Position
void resetScr(uint32_t bgColor) {
	// Reset SSFN Cursor Position
	ssfn_dst.x = ssfn_dst.y = 0;

	// Clear Screen With Color
	ssfn_dst.bg = bgColor;
	for(size_t y = 0; y < gfxInfo.fbHeight; y++) {
		for(size_t x = 0; x < gfxInfo.fbWidth; x++) {
			drawPixel(x, y, bgColor);
		}
	}
}

// Render Primitives
void drawCircle(int centerX, int centerY, int radius, uint32_t color) {
	for(int y = centerY - radius; y <= centerY + radius; y++) {
		for(int x = centerX - radius; x <= centerX + radius; x++) {
			if((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY) <= radius * radius) {
				drawPixel(x, y, color);
			}
		}
	}
}

// Text Functions
void setTextColor(uint32_t fgColor) {ssfn_dst.fg = fgColor;}
void _putchar(char character) {
	// This Function is A Putchar Implementation Required For Printf
	// Handle Special Characters
	switch(character) {
		// Newline
		case '\n':
			ssfn_dst.x = 0;
			if((++ssfn_dst.y + gfxInfo.glyphHeight) > (int)gfxInfo.fbHeight) {
				ssfn_dst.y = 0;
			} else {
				ssfn_dst.y += gfxInfo.glyphHeight;
			}
			break;

		// Tab
		case '\t':
			if((ssfn_dst.x + gfxInfo.glyphWidth) > (int)gfxInfo.fbWidth) {
				ssfn_dst.x = 0;
				if((ssfn_dst.y + gfxInfo.glyphHeight) > (int)gfxInfo.fbHeight) {
					ssfn_dst.y = 0;
				} else {
					ssfn_dst.y += gfxInfo.glyphHeight;
				}
			}

			for(uint8_t i = 0; i < 4; i++) {
				ssfn_putc(' ');
			}
			break;

		// Normal Character - Just Draw it
		default:
			if(++ssfn_dst.x > (int)gfxInfo.fbWidth) {
				ssfn_dst.x = 0;
				if((ssfn_dst.y + gfxInfo.glyphHeight) > (int)gfxInfo.fbHeight) {
					ssfn_dst.y = 0;
				} else {
					ssfn_dst.y += gfxInfo.glyphHeight;
				}
			}

			ssfn_putc(character);
			break;
	}
}
