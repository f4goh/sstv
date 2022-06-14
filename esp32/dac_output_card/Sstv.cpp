/* 
 * File:   Sstv.cpp
 * Author: ale
 * 
 * Created on 19 septembre 2021, 11:50
 * 
 */

#include "Sstv.h"

const SSTVMode_t Scottie1
{
    SSTV_SCOTTIE_1, //viscode
            320, //width
            256, //height
            432, //scanPixelLen 432*0.98 = 424 
            7, //numTones
            {
        {GENERIC, 1500, 1500},
        {SCAN_GREEN, 0, 0},
        {GENERIC, 1500, 1500},
        {SCAN_BLUE, 0, 0},
        {GENERIC, 9000, 1200},
        {GENERIC, 1500, 1500},
        {SCAN_RED, 0, 0}}
};


const SSTVMode_t Scottie2{
    SSTV_SCOTTIE_2,
    320,
    256,
    275,
    7,
    {
        {GENERIC, 1500, 1500},
        {SCAN_GREEN, 0, 0},
        {GENERIC, 1500, 1500},
        {SCAN_BLUE, 0, 0},
        {GENERIC, 9000, 1200},
        {GENERIC, 1500, 1500},
        {SCAN_RED, 0, 0}
    }
};

const SSTVMode_t ScottieDX{
    SSTV_SCOTTIE_DX,
    320,
    256,
    1080,
    7,
    {
        {GENERIC, 1500, 1500},
        {SCAN_GREEN, 0, 0},
        {GENERIC, 1500, 1500},
        {SCAN_BLUE, 0, 0},
        {GENERIC, 9000, 1200},
        {GENERIC, 1500, 1500},
        {SCAN_RED, 0, 0}
    }
};

const SSTVMode_t Martin1{
    SSTV_MARTIN_1,
    320,
    256,
    458,
    8,
    {
        {GENERIC, 4862, 1200},
        {GENERIC, 572, 1500},
        {SCAN_GREEN, 0, 0},
        {GENERIC, 572, 1500},
        {SCAN_BLUE, 0, 0},
        {GENERIC, 572, 1500},
        {SCAN_RED, 0, 0},
        {GENERIC, 572, 1500}
    }
};

const SSTVMode_t Martin2{
    SSTV_MARTIN_2,
    320,
    256,
    229,
    8,
    {
        {GENERIC, 4862, 1200},
        {GENERIC, 572, 1500},
        {SCAN_GREEN, 0, 0},
        {GENERIC, 572, 1500},
        {SCAN_BLUE, 0, 0},
        {GENERIC, 572, 1500},
        {SCAN_RED, 0, 0},
        {GENERIC, 572, 1500}
    }
};

const SSTVMode_t Wrasse{
    SSTV_WRASSE_SC2_180,
    320,
    256,
    734,
    5,
    {
        {GENERIC, 5523, 1200},
        {GENERIC, 500, 1500},
        {SCAN_RED, 0, 0},
        {SCAN_GREEN, 0, 0},
        {SCAN_BLUE, 0, 0}
    }
};

const SSTVMode_t PasokonP3{
    SSTV_PASOKON_P3,
    640,
    496,
    208,
    7,
    {
        {GENERIC, 5208, 1200},
        {GENERIC, 1042, 1500},
        {SCAN_RED, 0, 0},
        {GENERIC, 1042, 1500},
        {SCAN_GREEN, 0, 0},
        {GENERIC, 1042, 1500},
        {SCAN_BLUE, 0, 0}
    }
};

const SSTVMode_t PasokonP5{
    SSTV_PASOKON_P5,
    640,
    496,
    312,
    7,
    {
        {GENERIC, 7813, 1200},
        {GENERIC, 1563, 1500},
        {SCAN_RED, 0, 0},
        {GENERIC, 1563, 1500},
        {SCAN_GREEN, 0, 0},
        {GENERIC, 1563, 1500},
        {SCAN_BLUE, 0, 0}
    }
};

const SSTVMode_t PasokonP7{
    SSTV_PASOKON_P7,
    640,
    496,
    417,
    7,
    {
        {GENERIC, 10417, 1200},
        {GENERIC, 2083, 1500},
        {SCAN_RED, 0, 0},
        {GENERIC, 2083, 1500},
        {SCAN_GREEN, 0, 0},
        {GENERIC, 2083, 1500},
        {SCAN_BLUE, 0, 0}
    }
};



Sstv::Sstv() :
Dds()
{
    
}

Sstv::Sstv(const Sstv& orig) {
}

Sstv::~Sstv() {
}

void Sstv::begin(const SSTVMode_t &_mode, float correction)
{
mode = _mode;    
  // apply correction factor to all timings
  mode.scanPixelLen *= correction;
  for(uint8_t i = 0; i < mode.numTones; i++) {
    mode.tones[i].len *= correction;
  }
  Serial.println(mode.visCode);
  Dds::begin();
}
        
void Sstv::idle() {
  Dds::incrementPhase = Dds::computeIncrementPhase(SSTV_TONE_LEADER);   
}

void Sstv::tone(float freq, uint32_t len) {
    //uint32_t start = micros();
    Dds::incrementPhase = Dds::computeIncrementPhase(freq); 
    delayMicroseconds(len);                 //solution simple
   //while(micros() - start < len) {        //solution alternative a tester
   // yield();
   //}     
}

void Sstv::sendHeader() {
  // save first header flag for Scottie modes
  firstLine = true;

  // send the first part of header (leader-break-leader)
  tone(SSTV_TONE_LEADER, SSTV_HEADER_LEADER_LENGTH);
  tone(SSTV_TONE_BREAK, SSTV_HEADER_BREAK_LENGTH);
  tone(SSTV_TONE_LEADER, SSTV_HEADER_LEADER_LENGTH);

  // VIS start bit
  tone(SSTV_TONE_BREAK, SSTV_HEADER_BIT_LENGTH);

  // VIS code
  uint8_t parityCount = 0;
  for(uint8_t mask = 0x01; mask < 0x80; mask <<= 1) {
    if(mode.visCode & mask) {
      tone(SSTV_TONE_VIS_1, SSTV_HEADER_BIT_LENGTH);
      parityCount++;
    } else {
      tone(SSTV_TONE_VIS_0, SSTV_HEADER_BIT_LENGTH);
    }
  }

  // VIS parity
  if(parityCount % 2 == 0) {
    // even parity
    tone(SSTV_TONE_VIS_0, SSTV_HEADER_BIT_LENGTH);
  } else {
    // odd parity
    tone(SSTV_TONE_VIS_1, SSTV_HEADER_BIT_LENGTH);
  }

  // VIS stop bit
  tone(SSTV_TONE_BREAK, SSTV_HEADER_BIT_LENGTH);
}

void Sstv::sendLine(uint32_t* imgLine) {
  // check first line flag in Scottie modes
  if(firstLine && ((mode.visCode == SSTV_SCOTTIE_1) || (mode.visCode == SSTV_SCOTTIE_2) || (mode.visCode == SSTV_SCOTTIE_DX))) {
    firstLine = false;

    // send start sync tone
    tone(SSTV_TONE_BREAK, 9000);
  }

  // send all tones in sequence
  for(uint8_t i = 0; i < mode.numTones; i++) {
    if((mode.tones[i].tt == GENERIC) && (mode.tones[i].len > 0)) {
      // sync/porch tones
      tone(mode.tones[i].freq, mode.tones[i].len);
    } else {
      // scan lines
      for(uint16_t j = 0; j < mode.width; j++) {
        uint32_t color = imgLine[j];
        switch(mode.tones[i].tt) {
          case(SCAN_RED):
            color &= 0x00FF0000;
            color >>= 16;
            break;
          case(SCAN_GREEN):
            color &= 0x0000FF00;
            color >>= 8;
            break;
          case(SCAN_BLUE):
            color &= 0x000000FF;
            break;
          case(GENERIC):
            break;
        }
        tone(SSTV_TONE_BRIGHTNESS_MIN + ((float)color * 3.1372549), mode.scanPixelLen);
      }
    }
  }
}

uint16_t Sstv::getPictureHeight() {
  return(mode.height);
}

void Sstv::standby(){
  Dds::off();  
}
