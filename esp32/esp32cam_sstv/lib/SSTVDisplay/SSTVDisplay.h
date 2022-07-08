/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SSTVDisplay.h
 * Author: ale
 *
 * Created on 8 juillet 2022, 06:15
 */

#ifndef SSTVDISPLAY_H
#define SSTVDISPLAY_H

#include <Arduino.h>
#include "SSTVDisplayFonts.h"
#include "Sstv.h"


// Header Values
#define JUMPTABLE_BYTES 4

#define JUMPTABLE_LSB   1
#define JUMPTABLE_SIZE  2
#define JUMPTABLE_WIDTH 3
#define JUMPTABLE_START 4

#define WIDTH_POS 0
#define HEIGHT_POS 1
#define FIRST_CHAR_POS 2
#define CHAR_NUM_POS 3

typedef enum  {
  TEXT_ALIGN_LEFT = 0,
  TEXT_ALIGN_RIGHT = 1,
  TEXT_ALIGN_CENTER = 2,
  TEXT_ALIGN_CENTER_BOTH = 3
} textAlignment_t;



typedef struct  {
  uint8_t r; 
  uint8_t v; 
  uint8_t b;
}colorRGB_t;

typedef struct  {
  uint8_t y; 
  uint8_t u; 
  uint8_t v;
}colorYUV_t;



typedef char (*FontTableLookupFunction)(const uint8_t ch);
char DefaultFontTableLookup(const uint8_t ch);


class SSTVDisplay {
public:
    SSTVDisplay();
    SSTVDisplay(const SSTVDisplay& orig);
    virtual ~SSTVDisplay();
    
    uint16_t width(void) const { return sstvWidth; };
    uint16_t height(void) const { return sstvHeight; };
        
    
     // Draws a string at the given location, returns how many chars have been written
    uint16_t drawString(int16_t x, int16_t y, const String &text,uint8_t *_ptrImage,modeCoul _modeC);
    
        // Returns the width of the const char* with the current
    // font settings
    uint16_t getStringWidth(const char* text, uint16_t length, bool utf8 = false);
    
    
    // Set the function that will convert utf-8 to font table index
    void setFontTableLookupFunction(FontTableLookupFunction function);
    
    
    
private:
    uint8_t *ptrImage;
    modeCoul modeC;
    
    
    uint16_t  sstvWidth;
    uint16_t  sstvHeight;
    
    textAlignment_t   textAlignment;
    colorRGB_t         colorRGB;
    colorYUV_t         colorYUV;
    uint16_t color565;
    
    
    const uint8_t	 *fontData;
    
    uint16_t testCoul(float coul);
    uint16_t RGBto565(colorRGB_t color);
    colorYUV_t RGBtoYUV(colorRGB_t color);
    
    
    
    
    void inline drawInternal(int16_t xMove, int16_t yMove, int16_t width, int16_t height, const uint8_t *data, uint16_t offset, uint16_t bytesInData) __attribute__((always_inline));

    uint16_t drawStringInternal(int16_t xMove, int16_t yMove, const char* text, uint16_t textLength, uint16_t textWidth, bool utf8);
    
    FontTableLookupFunction fontTableLookupFunction;
    
    
};

#endif /* SSTVDISPLAY_H */

