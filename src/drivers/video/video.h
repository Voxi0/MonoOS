#ifndef VIDEO_H
#define VIDEO_H

// Standard Freestanding Headers
#include<stdint.h>

// Graphics Struct Definition
struct gfx_struct {
    uint32_t* fbAddress;
    uint64_t fbWidth, fbHeight;
    uint64_t fbPitch;
    int glyphWidth, glyphHeight;
};

// Convert RGB/RGBA to Hex Color Code
uint32_t rgbToHex(int r, int g, int b);
uint32_t rgbaToHex(int r, int g, int b, int a);

// Video Functions - Self Explanatory
void initGFX(uint32_t* fbAddress, uint64_t fbWidth, uint64_t fbHeight, uint64_t fbPitch);
void drawPixel(int xPos, int yPos, uint32_t color);
void resetScreen(const uint32_t bgColor);
void drawCircle(int centerX, int centerY, int radius, uint32_t color);
void drawChar(uint32_t unicode, uint64_t xPos, uint64_t yPos, uint32_t textColor);
void printStr(char* string, uint32_t textColor);

#endif
