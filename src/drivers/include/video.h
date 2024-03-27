#ifndef VIDEO_H
#define VIDEO_H

// Freestanding Headers
#include<stddef.h>
#include<stdint.h>

// Printf
#include<tiny-printf/printf.h>

// GFX Struct - Holds Vital Information Required to Draw Stuff Into Framebuffer
struct gfxStruct {
	uint32_t* fbAddress;
	size_t fbWidth, fbHeight, fbPitch;
	int glyphWidth, glyphHeight;
};

// Convert RGB/RGBA to Hex Color Code
uint32_t rgbToHex(int red, int green, int blue);
uint32_t rgbaToHex(int red, int green, int blue, int alpha);

// Self-Explanatory Graphics Functions
void initGFX(uint32_t* fbAddr, size_t fbWidth, size_t fbHeight, size_t fbPitch);
void drawPixel(int xPos, int yPos, uint32_t color);
void resetScr(uint32_t bgColor);

// Render Primitives
void drawCircle(int centerX, int centerY, int radius, uint32_t color);

// Text Functions
void setTextColor(uint32_t fgColor);

#endif
