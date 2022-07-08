/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SSTVDisplay.cpp
 * Author: ale
 * 
 * Created on 8 juillet 2022, 06:15
 * https://www.mikekohn.net/file_formats/yuv_rgb_converter.php
 */

#include "SSTVDisplay.h"

SSTVDisplay::SSTVDisplay() {
        sstvWidth = 320;                // ajouter setter 
	sstvHeight = 240;               // ajouter setter 
	colorRGB={255,0,0};             // ajouter setter           
	textAlignment = TEXT_ALIGN_LEFT; //non testé
	//fontData = ArialMT_Plain_24;
        fontData = DejaVu_Sans_Bold_40; //ajouter setter
        fontTableLookupFunction = DefaultFontTableLookup;
}

SSTVDisplay::SSTVDisplay(const SSTVDisplay& orig) {
}

SSTVDisplay::~SSTVDisplay() {
}

uint16_t SSTVDisplay::RGBto565(colorRGB_t color){
    uint16_t r,v,b;
    r=(color.r>>3)<<11;
    v=(color.v>>2)<<5;
    b=color.b>>3;
    return r|v|b;
}

uint16_t SSTVDisplay::testCoul(float coul){
    if (coul>255)return 255;
    if (coul<0) return 0;
    return (int)coul;
}

colorYUV_t SSTVDisplay::RGBtoYUV(colorRGB_t color){
    float Y,U,V;
    colorYUV_t px;
    
    Y = 16.0 + (.003906 * ((65.738 * color.r) + (129.057 * color.v) + (25.064 * color.b)));
    V = 128.0 + (.003906 * ((112.439 * color.r) + (-94.154 * color.v) + (-18.285 * color.b)));
    U = 128.0 + (.003906 * ((-37.945 * color.r) + (-74.494 * color.v) + (112.439 * color.b)));
    px.y=testCoul(Y);
    px.u=testCoul(U);
    px.v=testCoul(V);
    //Serial.println(px.y);
    //Serial.println(px.u);
    //Serial.println(px.v);
    return px;
}
        



uint16_t SSTVDisplay::drawString(int16_t xMove, int16_t yMove, const String &strUser,uint8_t *_ptrImage,modeCoul _modeC) {
  uint16_t lineHeight = pgm_read_byte(fontData + HEIGHT_POS);
  ptrImage=_ptrImage;
  modeC= _modeC;
  
  // char* text must be freed!
  if (modeC==RGB){
      color565=RGBto565(colorRGB);
  }else
  {
      colorYUV=RGBtoYUV(colorRGB);
  }
  
  char* text = strdup(strUser.c_str());
  if (!text) {
    Serial.println("[OLEDDISPLAY][drawString] Can't allocate char array.\n");
    return 0;
  }

  uint16_t yOffset = 0;
  // If the string should be centered vertically too
  // we need to now how heigh the string is.
  if (textAlignment == TEXT_ALIGN_CENTER_BOTH) {
    uint16_t lb = 0;
    // Find number of linebreaks in text
    for (uint16_t i=0;text[i] != 0; i++) {
      lb += (text[i] == 10);
    }
    // Calculate center
    yOffset = (lb * lineHeight) / 2;
  }

  uint16_t charDrawn = 0;
  uint16_t line = 0;
  char* textPart = strtok(text,"\n");
  while (textPart != NULL) {
    uint16_t length = strlen(textPart);
    charDrawn += drawStringInternal(xMove, yMove - yOffset + (line++) * lineHeight, textPart, length, getStringWidth(textPart, length, true), true);
    textPart = strtok(NULL, "\n");
  }
  free(text);
  return charDrawn;
}

uint16_t SSTVDisplay::drawStringInternal(int16_t xMove, int16_t yMove, const char* text, uint16_t textLength, uint16_t textWidth, bool utf8) {
  uint8_t textHeight       = pgm_read_byte(fontData + HEIGHT_POS);
  uint8_t firstChar        = pgm_read_byte(fontData + FIRST_CHAR_POS);
  uint16_t sizeOfJumpTable = pgm_read_byte(fontData + CHAR_NUM_POS)  * JUMPTABLE_BYTES;

  uint16_t cursorX         = 0;
  uint16_t cursorY         = 0;
  uint16_t charCount       = 0;

  switch (textAlignment) {
    case TEXT_ALIGN_CENTER_BOTH:
      yMove -= textHeight >> 1;
    // Fallthrough
    case TEXT_ALIGN_CENTER:
      xMove -= textWidth >> 1; // divide by 2
      break;
    case TEXT_ALIGN_RIGHT:
      xMove -= textWidth;
      break;
    case TEXT_ALIGN_LEFT:
      break;
  }

  // Don't draw anything if it is not on the screen.
  if (xMove + textWidth  < 0 || xMove >= this->width() ) {return 0;}
  if (yMove + textHeight < 0 || yMove >= this->height()) {return 0;}

  for (uint16_t j = 0; j < textLength; j++) {
    int16_t xPos = xMove + cursorX;
    int16_t yPos = yMove + cursorY;
    if (xPos > this->width())
      break; // no need to continue
    charCount++;

    uint8_t code;
    if (utf8) {
      code = (this->fontTableLookupFunction)(text[j]);
      if (code == 0)
        continue;
    } else
      code = text[j];
    if (code >= firstChar) {
      uint8_t charCode = code - firstChar;

      // 4 Bytes per char code
      uint8_t msbJumpToChar    = pgm_read_byte( fontData + JUMPTABLE_START + charCode * JUMPTABLE_BYTES );                  // MSB  \ JumpAddress
      uint8_t lsbJumpToChar    = pgm_read_byte( fontData + JUMPTABLE_START + charCode * JUMPTABLE_BYTES + JUMPTABLE_LSB);   // LSB /
      uint8_t charByteSize     = pgm_read_byte( fontData + JUMPTABLE_START + charCode * JUMPTABLE_BYTES + JUMPTABLE_SIZE);  // Size
      uint8_t currentCharWidth = pgm_read_byte( fontData + JUMPTABLE_START + charCode * JUMPTABLE_BYTES + JUMPTABLE_WIDTH); // Width

      // Test if the char is drawable
      if (!(msbJumpToChar == 255 && lsbJumpToChar == 255)) {
        // Get the position of the char data
        uint16_t charDataPosition = JUMPTABLE_START + sizeOfJumpTable + ((msbJumpToChar << 8) + lsbJumpToChar);
        drawInternal(xPos, yPos, currentCharWidth, textHeight, fontData, charDataPosition, charByteSize);
      }

      cursorX += currentCharWidth;
    }
  }
  return charCount;
}

/*
 codage de la police
 *exemple du caractère A 65 en font ArialMT_Plain_10 (pour la sstv c'est trop petit)
 *  les caractères dans la progmeme sont les suvants :
 *  0x01, 0x0A, 0x0E, 0x07,  // 65:266
 *  0x00,0x02,0xC0,0x01,0xB0,0x00,0x88,0x00,0xB0,0x00,0xC0,0x01,0x00,0x02,  // 65 A
 *  la taille est de 7 octets de large par 2 octets de haut ce qui donne 14 octets pour le caractère
 *  il faut lire les octets de haut en bas puis de gauche a droite
 *  0x00,0xC0,0xB0,0x88,0xB0,0xC0,0x00, ligne 0
 *  0x02,0x01,0x00,0x00,0x00,0x01,0x02, ligne 1
 *  en binaire cela donne
 * 0000000 lsb ligne 0
 * 0000000
 * 0000000
 * 0001000
 * 0010100
 * 0010100
 * 0100010
 * 0111110 msb
 
 * 0100010 lsb ligne 1
 * 1000001
 * 0000000
 * 0000000
 * 0000000
 * 0000000
 * 0000000
 * 0000000 msb
 * 
 */


void inline SSTVDisplay::drawInternal(int16_t xMove, int16_t yMove, int16_t width, int16_t height, const uint8_t *data, uint16_t offset, uint16_t bytesInData) {
    if (width < 0 || height < 0) return;
    if (yMove + height < 0 || yMove > this->height()) return; // fenetrage anti debordement à vérifier
    if (xMove + width < 0 || xMove > this->width()) return;   //fonctionne que si le caractère en cours est à déja l'extérieur du cadre

    uint8_t rasterHeight = 1 + ((height - 1) >> 3); // fast ceil(height / 8.0)

    bytesInData = bytesInData == 0 ? width * rasterHeight : bytesInData;


    int32_t offsetImage = (yMove * this->width() + xMove) *2; //2 octets par pixels

    /*
        Serial.println(color565,HEX);
        Serial.println("------");
        Serial.print(xMove);
        Serial.print(',');
        Serial.print(yMove);
        Serial.print(',');
        Serial.print(width);
        Serial.print(',');
        Serial.print(height);
        Serial.print(',');       
        Serial.print(offsetImage);
        Serial.print(',');       
        Serial.print(rasterHeight); //nb d'octets en hauteur
        Serial.print(',');
        Serial.print(bytesInData);
        Serial.print(',');
        Serial.println(bytesInData/rasterHeight); //nb d'octets en largeur
     */

    uint16_t i = 0;
    int32_t offsetImageCourant;
    uint8_t currentByte;

    for (uint16_t x = 0; x < bytesInData / rasterHeight; x++) { //largeur
        offsetImageCourant = offsetImage + x * 2; //ajuste l'offset de la mémoire en X
        for (uint16_t y = 0; y < rasterHeight; y++) { //hauteur
            currentByte = pgm_read_byte(data + offset + i); //lecture d'un octet dans la table de la police
            i++;
            for (uint8_t b = 0; b < 8; b++) {
                offsetImageCourant += this->width()*2; //sauf au pixel du dessous
                if ((currentByte & 1) == 1) {
                    if (modeC == RGB) {
                        //mise a 1 de la couleur en écrasant les 2 octets de la mémoire RGB565                            
                        ptrImage[offsetImageCourant] = color565 >> 8;
                        ptrImage[offsetImageCourant + 1] = color565 & 0xff;
                    } else { //YUV422 = Y0 U0      Y1 V0
                        if ((offsetImageCourant/2)%2==0){  //Y0 U0
                             ptrImage[offsetImageCourant]=colorYUV.y;
                             ptrImage[offsetImageCourant+1]=colorYUV.u;
                        }else //Y1 V0
                        {
                             ptrImage[offsetImageCourant]=colorYUV.y;
                             ptrImage[offsetImageCourant+1]=colorYUV.v;
                        }
                    }
                }
                currentByte >>= 1;
            }
        }
    }
}


uint16_t SSTVDisplay::getStringWidth(const char* text, uint16_t length, bool utf8) {
  uint16_t firstChar        = pgm_read_byte(fontData + FIRST_CHAR_POS);

  uint16_t stringWidth = 0;
  uint16_t maxWidth = 0;

  for (uint16_t i = 0; i < length; i++) {
    char c = text[i];
    if (utf8) {
      c = (this->fontTableLookupFunction)(c);
      if (c == 0)
        continue;
    }
    stringWidth += pgm_read_byte(fontData + JUMPTABLE_START + (c - firstChar) * JUMPTABLE_BYTES + JUMPTABLE_WIDTH);
    if (c == 10) {
      maxWidth = max(maxWidth, stringWidth);
      stringWidth = 0;
    }
  }

  return max(maxWidth, stringWidth);
}

void SSTVDisplay::setFontTableLookupFunction(FontTableLookupFunction function) {
  this->fontTableLookupFunction = function;
}

char DefaultFontTableLookup(const uint8_t ch) {
    // UTF-8 to font table index converter
    // Code form http://playground.arduino.cc/Main/Utf8ascii
	static uint8_t LASTCHAR;

	if (ch < 128) { // Standard ASCII-set 0..0x7F handling
		LASTCHAR = 0;
		return ch;
	}

	uint8_t last = LASTCHAR;   // get last char
	LASTCHAR = ch;

	switch (last) {    // conversion depnding on first UTF8-character
		case 0xC2: return (uint8_t) ch;
		case 0xC3: return (uint8_t) (ch | 0xC0);
		case 0x82: if (ch == 0xAC) return (uint8_t) 0x80;    // special case Euro-symbol
	}

	return (uint8_t) 0; // otherwise: return zero, if character has to be ignored
}