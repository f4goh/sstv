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
timer(NULL)
{
 //begin();  //report à la méthode tx
 //pinMode(SYNC,OUTPUT);
 anchor = this;
 timer = timerBegin(0, 80, true);
 timerAttachInterrupt(timer, Sstv::marshall, true);
}

Sstv::Sstv(const Sstv& orig) {
}

Sstv::~Sstv() {
}

/**
   @brief   Sstv::tx(const SSTVMode_t &_mode)
   @details demarre une trasmission sstv
   @param   &_mode mode de trasmission   
 */

void Sstv::tx(const SSTVMode_t &_mode) {
    begin();
    mode = _mode;    
    Serial.println(mode.visCode);
    irqDone = 0;
    timerAlarmWrite(timer, 1000, true);
    timerAlarmEnable(timer);
    timerMux = portMUX_INITIALIZER_UNLOCKED;
}

/**
   @brief   Sstv::idle()
   @details génère une fréquence de 1500hz + la fréquence VFO
 */


void Sstv::idle() {
  setfreq(FREQ + SSTV_TONE_BREAK,0);
}


/**
   @brief   Sstv::toneUs(float freq, uint32_t len)
   @details déchenche une temporisation via l'interruption timer
   @param   freq : fréquence bf
            len : durée en µs   
 */

void Sstv::toneUs(float freq, uint32_t len) {
    while (!irqDone) {
    }
    portENTER_CRITICAL(&timerMux);
    frequency = FREQ + freq;
    tPeriod = len;
    irqDone=0;
    portEXIT_CRITICAL(&timerMux);
}

/**
   @brief   Sstv::sendHeader()
   @details envoi le code VIS qui identifie le mode de transmission sstv
 */

void Sstv::sendHeader() {
  // save first header flag for Scottie modes
  firstLine = true;

  // send the first part of header (leader-break-leader)
  toneUs(SSTV_TONE_LEADER, SSTV_HEADER_LEADER_LENGTH);
  toneUs(SSTV_TONE_BREAK, SSTV_HEADER_BREAK_LENGTH);
  toneUs(SSTV_TONE_LEADER, SSTV_HEADER_LEADER_LENGTH);

  // VIS start bit
  toneUs(SSTV_TONE_BREAK, SSTV_HEADER_BIT_LENGTH);

  // VIS code
  uint8_t parityCount = 0;
  for(uint8_t mask = 0x01; mask < 0x80; mask <<= 1) {
    if(mode.visCode & mask) {
      toneUs(SSTV_TONE_VIS_1, SSTV_HEADER_BIT_LENGTH);
      parityCount++;
    } else {
      toneUs(SSTV_TONE_VIS_0, SSTV_HEADER_BIT_LENGTH);
    }
  }

  // VIS parity
  if(parityCount % 2 == 0) {
    // even parity
    toneUs(SSTV_TONE_VIS_0, SSTV_HEADER_BIT_LENGTH);
  } else {
    // odd parity
    toneUs(SSTV_TONE_VIS_1, SSTV_HEADER_BIT_LENGTH);
  }

  // VIS stop bit
  toneUs(SSTV_TONE_BREAK, SSTV_HEADER_BIT_LENGTH);
}

/**
   @brief   Sstv::sendLine(int idxLine)
   @details génére une ligne sstv
   @param   idxLine : index dans la mémoire progmem
            ptr : pointeur pour la mémoire de la camera
            imgtype : type de transmission enum  PROG_MEM ou CAMERA
 */

void Sstv::sendLine(int idxLine,uint8_t *ptr,imageType imgtype) {
  // check first line flag in Scottie modes
    if (firstLine && ((mode.visCode == SSTV_SCOTTIE_1) || (mode.visCode == SSTV_SCOTTIE_2) || (mode.visCode == SSTV_SCOTTIE_DX))) {
        firstLine = false;

        // send start sync tone
        toneUs(SSTV_TONE_BREAK, 9000);
    }
    // send all tones in sequence
    uint16_t color;
    for (uint8_t i = 0; i < mode.numTones; i++) {
        if ((mode.tones[i].tt == GENERIC) && (mode.tones[i].len > 0)) {
            // sync/porch tones
            toneUs(mode.tones[i].freq, mode.tones[i].len);
        } else {
            // scan lines
            for (uint16_t j = 0; j < mode.width; j++) {
                if (imgtype == PROG_MEM) {
                    if (idxLine >= 0) {
                        color = pgm_read_word(&(imageTest[idxLine + j]));
                    } else {
                        color = pgm_read_word(&(mire[j % 320]));
                    }
                } else {
                    color = (ptr[j * 2] << 8) + ptr[j * 2 + 1]; //ici récup word pixel msb+lsb   
                }
                switch (mode.tones[i].tt) {
                    case(SCAN_RED):
                        color >>= 11;
                        color <<= 3;
                        break;
                    case(SCAN_GREEN):
                        color = (color >> 5) & 0x3f;
                        color <<= 2;
                        break;
                    case(SCAN_BLUE):
                        color &= 0x1F;
                        color <<= 3;
                        break;
                    case(GENERIC):
                        break;
                }
                toneUs(SSTV_TONE_BRIGHTNESS_MIN + ((float) color * 3.1372549), mode.scanPixelLen);
            }
        }
    }
}


/**
   @brief   Sstv::standby()
   @details stop toute transmission
 */

void Sstv::standby(){
  timerAlarmDisable(timer);  
  AD9850SPI::setfreq(0,0);
}

void Sstv::marshall() {
    anchor->interuption();
}

/**
   @brief   IRAM_ATTR Sstv::interuption()
   @details irq avec syncro oscillo
 */

void IRAM_ATTR Sstv::interuption() {
  //digitalWrite(SYNC, digitalRead(SYNC) ^ 1);
  setfreq(frequency,0); //0
  timerAlarmWrite(timer, tPeriod, true);
  portENTER_CRITICAL_ISR(&timerMux);
  irqDone=1;
  portEXIT_CRITICAL_ISR(&timerMux);
}

/**
   @brief   Sstv::sendMire()
   @details envoie une mire
 */

void Sstv::sendMire(){
   Serial.println(F("[SSTV] Sending mire  picture ... "));
    sendHeader(); // send synchronization header first
    // send all picture lines
    for (uint16_t i = 0; i < mode.height; i++) {    
        sendLine(-1,NULL,PROG_MEM);  //for pattern
    }
    standby(); // turn off transmitter
    Serial.println(F("done!"));      
}

/**
   @brief   Sstv::sendImg()
   @details envoie une image en progmem
 */


void Sstv::sendImg() {
    Serial.println(F("[SSTV] Sending test image picture ... "));
    sendHeader(); // send synchronization header first
    // send all picture lines    
    int idxLine=0;
    for (uint16_t i = 0; i < mode.height; i++) {    
        sendLine(idxLine,NULL,PROG_MEM);   // for progmem image
        idxLine+=mode.width;
    }
    standby(); // turn off transmitter
    Serial.println(F("done!"));
}

void Sstv::sendCamera(uint8_t *ptr) {
    Serial.println(F("[SSTV] Sending camera image picture ... "));
    sendHeader(); // send synchronization header first
    // send all picture lines    
    for (uint16_t i = 0; i < mode.height; i++) {    
        sendLine(0,ptr,CAMERA);
        ptr+=mode.width*2;  //2 octets par pixels
    }
    standby(); // turn off transmitter
    Serial.println(F("done!"));
}



Sstv* Sstv::anchor = NULL;