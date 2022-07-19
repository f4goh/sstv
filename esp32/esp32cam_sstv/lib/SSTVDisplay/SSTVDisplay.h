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
#include <Wire.h>
#include "driver/i2c.h"

#define SCCB_FREQ               CONFIG_SCCB_CLK_FREQ  /*!< I2C master frequency*/
#define WRITE_BIT               I2C_MASTER_WRITE      /*!< I2C master write */
#define READ_BIT                I2C_MASTER_READ       /*!< I2C master read */
#define ACK_CHECK_EN            0x1                   /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS           0x0                   /*!< I2C master will not check ack from slave */
#define ACK_VAL                 0x0                   /*!< I2C ack value */
#define NACK_VAL                0x1                   /*!< I2C nack value */


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

//#define I2C_SDA 26 //déja utilisé via la caméra
//#define I2C_SCL 27
#define I2C_SLAVE 0x70
#define I2C_PORT_NUM I2C_NUM_1


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


typedef struct
{
  float latitude;
  float longitude;
  uint32_t altitude;
} coordonnees;


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
    
    void addPosition();
    void I2Ctest();
    
    
    
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

