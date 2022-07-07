/* 
 * File:   Sstv.cpp
 * Author: ale
 * 
 * Created on 19 septembre 2021, 11:50
 * doc
 * https://github.com/xdsopl/qsstv/tree/master/qsstv/sstv
 * https://github.com/brainwagon/sstv-encoders
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

const SSTVMode_t Robot72{
    SSTV_ROBOT_72,
    320,
    240,
    431,
    9,
    {
        {GENERIC, 9000, 1200},
        {GENERIC, 3000, 1500},
        {SCAN_Y, 0, 0},
        {GENERIC, 4500, 1500},
        {GENERIC, 1500, 1900},
        {SCAN_R_Y, 0, 0},
        {GENERIC, 4500, 2300},
        {GENERIC, 1500, 1500},
        {SCAN_B_Y, 0, 0}        
    }
};

const SSTVMode_t Robot36{
    SSTV_ROBOT_36,
    320,
    240,
    275,
    12,
    {
        {GENERIC, 9000, 1200},
        {GENERIC, 3000, 1500},
        {SCAN_Y_EVEN, 0, 0},
        {GENERIC, 4500, 1500},
        {GENERIC, 1500, 1900},
        {SCAN_R_Y, 0, 0},
        {GENERIC, 9000, 1200},
        {GENERIC, 3000, 1500},
        {SCAN_Y_ODD, 0, 0},
        {GENERIC, 4500, 2300},
        {GENERIC, 1500, 1500},
        {SCAN_B_Y, 0, 0}        
    }
};

const SSTVMode_t PD50{
    SSTV_PD_50,
    320,
    256,
    286,
    6,
    {
        {GENERIC, 20000, 1200},
        {GENERIC, 2080, 1500},
        {SCAN_Y_EVEN, 0, 0},
        {SCAN_R_Y, 0, 0},       
        {SCAN_B_Y, 0, 0},        
        {SCAN_Y_ODD, 0, 0},
    }
};

const SSTVMode_t PD90{
    SSTV_PD_90,
    320,
    256,
    532,
    6,
    {
        {GENERIC, 20000, 1200},
        {GENERIC, 2080, 1500},
        {SCAN_Y_EVEN, 0, 0},
        {SCAN_R_Y, 0, 0},       
        {SCAN_B_Y, 0, 0},        
        {SCAN_Y_ODD, 0, 0},
    }
};

const SSTVMode_t MP73N{
    SSTV_MP_73_N,
    320,
    256,
    140,
    6,
    {
        {GENERIC, 9000, 1900},
        {GENERIC, 1000, 2100},
        {SCAN_Y_EVEN, 0, 0},
        {SCAN_R_Y, 0, 0},       
        {SCAN_B_Y, 0, 0},        
        {SCAN_Y_ODD, 0, 0},
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
  begin();
  setfreq(FREQ + SSTV_TONE_BREAK_L,0);
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
   @brief   Sstv::sendHeaderStandard()
   @details envoi le code VIS qui identifie le mode de transmission sstv
 */
//from qsstv code source
//{"MP73-Narrow" ,"MP73-N",   MP73N,   72.9665,320,256,128, 0x5C256D,0.00900,0.00000,0.00100,0.00000,0.00900,0.00000,0.00100,0.00000,0.,2172,128 },

void Sstv::sendHeaderNarrow() {
  int i,l;
  int t=0x5C256D;  //manque ce calcul avec mode.visCode
    l=24;
    toneUs(1900,300000);
    toneUs(2100,100000);
    toneUs(1900,22000); // startbit
    for (i=0;i<l;i++)
    {
      if((t&1)==1) toneUs(1900,22000);
      else toneUs(2100,22000);
      t>>=1;
    }  
}


void Sstv::sendHeaderStandard() {
  // save first header flag for Scottie modes
  firstLine = true;
  
  // send the first part of header (leader-break-leader)
  toneUs(SSTV_TONE_LEADER_L, SSTV_HEADER_LEADER_LENGTH);
  toneUs(SSTV_TONE_BREAK_L, SSTV_HEADER_BREAK_LENGTH);
  toneUs(SSTV_TONE_LEADER_L, SSTV_HEADER_LEADER_LENGTH);

  // VIS start bit
  toneUs(SSTV_TONE_BREAK_L, SSTV_HEADER_BIT_LENGTH);

  // VIS code
  uint8_t parityCount = 0;
  for(uint8_t mask = 0x01; mask < 0x80; mask <<= 1) {
    if(mode.visCode & mask) {
      toneUs(SSTV_TONE_VIS_1_L, SSTV_HEADER_BIT_LENGTH);
      parityCount++;
    } else {
      toneUs(SSTV_TONE_VIS_0_L, SSTV_HEADER_BIT_LENGTH);
    }
  }

  // VIS parity
  if(parityCount % 2 == 0) {
    // even parity
    toneUs(SSTV_TONE_VIS_0_L, SSTV_HEADER_BIT_LENGTH);
  } else {
    // odd parity
    toneUs(SSTV_TONE_VIS_1_L, SSTV_HEADER_BIT_LENGTH);
  }

  // VIS stop bit
  toneUs(SSTV_TONE_BREAK_L, SSTV_HEADER_BIT_LENGTH);
}


void Sstv::sendEndVis(){
    toneUs(SSTV_TONE_BRIGHTNESS_MAX_L, SSTV_HEADER_LEADER_LENGTH);
    toneUs(SSTV_TONE_BREAK_L, SSTV_HEADER_BREAK_LENGTH);
    toneUs(SSTV_TONE_BRIGHTNESS_MAX_L, SSTV_HEADER_END_LENGTH);
    toneUs(SSTV_TONE_BREAK_L, SSTV_HEADER_BIT_LENGTH);
}


/**
   @brief   Sstv::sendLine(int idxLine)
   @details génére une ligne sstv
   @param   idxLine : index dans la mémoire progmem
            ptr : pointeur pour la mémoire de la camera
            imgtype : type de transmission enum  PROG_MEM ou CAMERA
 */

void Sstv::sendLineRGB(int idxLine,uint8_t *ptr,imageType imgtype) {
  // check first line flag in Scottie modes
    if (firstLine && ((mode.visCode == SSTV_SCOTTIE_1) || (mode.visCode == SSTV_SCOTTIE_2) || (mode.visCode == SSTV_SCOTTIE_DX))) {
        firstLine = false;

        // send start sync tone
        toneUs(SSTV_TONE_BREAK_L, 9000);
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
                        color = pgm_read_word(&(imageTestRGB[idxLine + j]));
                    } else {
                        color = pgm_read_word(&(mireRGB[j % 320]));
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
                toneUs(SSTV_TONE_BRIGHTNESS_MIN_L + ((float) color * 3.1372549), mode.scanPixelLen);
            }
        }
    }
}
//            BY    RY
//YUV422 = Y0 U0 Y1 V0

void Sstv::sendLineYUV(int idxLine, uint8_t *ptr, imageType imgtype) {
    // send all tones in sequence
    uint16_t color = 0;
    const word *addPgm;
    if (imgtype == PROG_MEM) {
        if (idxLine < 0) {
            addPgm = mireYUV;
            idxLine = 0;
        } else {
            addPgm = imageTestYUV;
        }
    }

    for (uint8_t i = 0; i < mode.numTones; i++) {
        if ((mode.tones[i].tt == GENERIC) && (mode.tones[i].len > 0)) {
            // sync/porch tones
            toneUs(mode.tones[i].freq, mode.tones[i].len);
        } else {
            // scan lines
            if (mode.visCode == SSTV_ROBOT_72) {
                switch (mode.tones[i].tt) {
                    case(SCAN_Y):
                        for (uint16_t j = 0; j < mode.width; j++) {
                            if (imgtype == PROG_MEM) color = pgm_read_word(&(addPgm[idxLine + j])) >> 8; //mire
                            else color = ptr[j * 2]; //camera
                            toneUs(SSTV_TONE_BRIGHTNESS_MIN_L + ((float) color * 3.1372549), mode.scanPixelLen);
                        }
                        break;
                    case(SCAN_R_Y):
                        for (uint16_t j = 0; j < mode.width / 2; j++) {
                            if (imgtype == PROG_MEM) color = pgm_read_word(&(addPgm[idxLine + j * 2 + 1])) & 0xff;
                            else color = ptr[j * 4 + 3];
                            toneUs(SSTV_TONE_BRIGHTNESS_MIN_L + ((float) color * 3.1372549), mode.scanPixelLen);
                        }
                        break;
                    case(SCAN_B_Y):
                        for (uint16_t j = 0; j < mode.width / 2; j++) {
                            if (imgtype == PROG_MEM) color = pgm_read_word(&(addPgm[idxLine + j * 2])) & 0xff;
                            else color = ptr[j * 4 + 1];
                            toneUs(SSTV_TONE_BRIGHTNESS_MIN_L + ((float) color * 3.1372549), mode.scanPixelLen);
                        }
                        break;
                    case(GENERIC):
                        break;
                }
            }
            if (mode.visCode == SSTV_ROBOT_36) {
                switch (mode.tones[i].tt) {
                    case(SCAN_Y_EVEN):
                        for (uint16_t j = 0; j < mode.width; j++) {
                            if (imgtype == PROG_MEM) color = pgm_read_word(&(addPgm[idxLine + j])) >> 8; //mire
                            else color = ptr[j * 2]; //ligne paire
                            toneUs(SSTV_TONE_BRIGHTNESS_MIN_L + ((float) color * 3.1372549), mode.scanPixelLen);
                        }
                        break;
                    case(SCAN_R_Y):
                        for (uint16_t j = 0; j < mode.width / 2; j++) {
                            if (imgtype == PROG_MEM) color = pgm_read_word(&(addPgm[idxLine + j * 2 + 1])) & 0xff;
                            else color = (ptr[j * 4 + 3] + ptr[mode.width * 2 + j * 4 + 3]) / 2; //moyenne des 2 V0 verticale
                            toneUs(SSTV_TONE_BRIGHTNESS_MIN_L + ((float) color * 3.1372549), mode.scanPixelLen);
                        }
                        break;
                    case(SCAN_Y_ODD):
                        for (uint16_t j = 0; j < mode.width; j++) {
                            if (imgtype == PROG_MEM) color = pgm_read_word(&(addPgm[idxLine + j])) >> 8;
                            else color = ptr[mode.width * 2 + j * 2]; //ligne impaire
                            toneUs(SSTV_TONE_BRIGHTNESS_MIN_L + ((float) color * 3.1372549), mode.scanPixelLen);
                        }
                        break;
                    case(SCAN_B_Y):
                        for (uint16_t j = 0; j < mode.width / 2; j++) {
                            if (imgtype == PROG_MEM) color = pgm_read_word(&(addPgm[idxLine + j * 2])) & 0xff;
                            else color = (ptr[j * 4 + 1] + ptr[mode.width * 2 + j * 4 + 1]) / 2; //moyenne des 2 U0 verticale
                            toneUs(SSTV_TONE_BRIGHTNESS_MIN_L + ((float) color * 3.1372549), mode.scanPixelLen);
                        }
                        break;
                    case(GENERIC):
                        break;
                }
            }
            if (mode.visCode == SSTV_PD_50 || mode.visCode == SSTV_PD_90) {
                switch (mode.tones[i].tt) {
                    case(SCAN_Y_EVEN):
                        for (uint16_t j = 0; j < mode.width; j++) {
                            if (imgtype == PROG_MEM) color = pgm_read_word(&(addPgm[idxLine + j])) >> 8; //mire
                            else color = ptr[j * 2]; //ligne paire
                            toneUs(SSTV_TONE_BRIGHTNESS_MIN_L + ((float) color * 3.1372549), mode.scanPixelLen);
                        }
                        break;
                    case(SCAN_R_Y):
                        for (uint16_t j = 0; j < mode.width / 2; j++) {
                            //color=128;
                            if (imgtype == PROG_MEM) color = pgm_read_word(&(addPgm[idxLine + j * 2 + 1])) & 0xff;
                            else color = (ptr[j * 4 + 3] + ptr[mode.width * 2 + j * 4 + 3]) / 2; //moyenne des 2 V0 verticale
                            toneUs(SSTV_TONE_BRIGHTNESS_MIN_L + ((float) color * 3.1372549), mode.scanPixelLen * 2);
                        }
                        break;
                    case(SCAN_Y_ODD):
                        for (uint16_t j = 0; j < mode.width; j++) {
                            if (imgtype == PROG_MEM) color = pgm_read_word(&(addPgm[idxLine + j])) >> 8;
                            else color = ptr[mode.width * 2 + j * 2]; //ligne impaire
                            toneUs(SSTV_TONE_BRIGHTNESS_MIN_L + ((float) color * 3.1372549), mode.scanPixelLen);
                        }
                        break;
                    case(SCAN_B_Y):
                        for (uint16_t j = 0; j < mode.width / 2; j++) {
                            //color=128;
                            if (imgtype == PROG_MEM) color = pgm_read_word(&(addPgm[idxLine + j * 2])) & 0xff;
                            else color = (ptr[j * 4 + 1] + ptr[mode.width * 2 + j * 4 + 1]) / 2; //moyenne des 2 U0 verticale
                            toneUs(SSTV_TONE_BRIGHTNESS_MIN_L + ((float) color * 3.1372549), mode.scanPixelLen * 2);
                        }
                        break;
                    case(GENERIC):
                        break;
                }
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

void Sstv::sendMireNarrow()
{
  Serial.print(F("[SSTV] Sending narrow mire vis picture ... "));
  sendHeaderNarrow();           
  if (mode.visCode == SSTV_MP_73_N){
             //a suivre 
        }  
  standby(); // turn off transmitter
  Serial.println(F("done!"));            
}

/**
   @brief   Sstv::sendMire()
   @details envoie une mire
 */

void Sstv::sendMire(modeCoul mCoul){
    Serial.print(F("[SSTV] Sending mire  picture ... "));
    sendHeaderStandard(); // send synchronization header first
    // send pattern lines
     uint16_t i;
    if (mCoul == YUV) {
        Serial.println(F("YUV"));
        if (mode.visCode == SSTV_ROBOT_72) {
            for (i = 0; i < mode.height; i++) {
                 sendLineYUV(-1, NULL, PROG_MEM); //for pattern                
            }
        }
        if (mode.visCode == SSTV_ROBOT_36 || mode.visCode == SSTV_PD_50 || mode.visCode == SSTV_PD_90) {
            for (i = 0; i < mode.height / 2; i++) {
                 sendLineYUV(-1, NULL, PROG_MEM); //for pattern                
            }
        }        
    }
    if (mCoul == RGB) {
        Serial.println(F("RGB"));
        for (i = 0; i < mode.height; i++) {
            sendLineRGB(-1, NULL, PROG_MEM); //for pattern
        }
    }
    sendEndVis();
    standby(); // turn off transmitter
    Serial.println(F("done!"));   
    //down();
}
/**
   @brief   Sstv::sendImg()
   @details envoie une image test depuis la progmem en RGB seulement
 */


void Sstv::sendImg(modeCoul mCoul) {
    Serial.print(F("[SSTV] Sending RGB test image picture ... "));
    sendHeaderStandard(); // send synchronization header first
    // send all picture lines   
    uint16_t i;
    int idxLine = 0;
     if (mCoul == YUV) {
        Serial.println(F("YUV"));
        if (mode.visCode == SSTV_ROBOT_72) {
            for (i = 0; i < mode.height; i++) {
                 sendLineYUV(idxLine, NULL, PROG_MEM); //for pattern 
                 idxLine += mode.width;
            }
        }
        if (mode.visCode == SSTV_ROBOT_36 || mode.visCode == SSTV_PD_50 || mode.visCode == SSTV_PD_90) {
            for (i = 0; i < mode.height / 2; i++) {
                 sendLineYUV(idxLine, NULL, PROG_MEM); //for pattern                
                 idxLine += mode.width*2;
            }
        }
    }
    if (mCoul == RGB) {
        Serial.println(F("RGB"));
        for (i = 0; i < mode.height; i++) {
            sendLineRGB(idxLine, NULL, PROG_MEM); // for progmem image
            idxLine += mode.width;
        }
    }
    sendEndVis();
    standby(); // turn off transmitter
    Serial.println(F("done!"));
    //down();
}

void Sstv::sendCameraRGB(uint8_t *ptr) {
    Serial.println(F("[SSTV] Sending camera RGB image picture ... "));
    sendHeaderStandard(); // send synchronization header first
    uint16_t i;
    // send all picture lines    
    for (uint16_t i = 0; i < mode.height-16; i++) {    
        sendLineRGB(0,ptr,CAMERA);
        ptr+=mode.width*2;  //2 octets par pixels
    }
    for ( i = 0; i < 16; i++) {    
         sendLineRGB(-1,NULL,PROG_MEM);  //for pattern       
    }
    sendEndVis();
    standby(); // turn off transmitter
    Serial.println(F("done!"));
    //down();
}

void Sstv::sendCameraYUV(uint8_t *ptr) {
    Serial.println(F("[SSTV] Sending camera YUV image picture ... "));
    sendHeaderStandard(); // send synchronization header first
    uint16_t i;
    // send all picture lines
    if (mode.visCode == SSTV_ROBOT_72) {
        for (i = 0; i < mode.height; i++) {
            sendLineYUV(0, ptr, CAMERA);
            ptr += mode.width * 2; //2 octets par pixels
        }
    }
    if (mode.visCode == SSTV_ROBOT_36) {
        for (i = 0; i < mode.height / 2; i++) {
            sendLineYUV(0, ptr, CAMERA);
            ptr += mode.width * 4; //2 octets par pixels et toutes les 2 lignes
        }
    }
    if (mode.visCode == SSTV_PD_50 || mode.visCode == SSTV_PD_90) {
        for (i = 0; i < (mode.height - 16) / 2; i++) {
            sendLineYUV(0, ptr, CAMERA);
            ptr += mode.width * 4; //2 octets par pixels et toutes les 2 lignes
        }
        for (i = 0; i < 16 / 2; i++) { //16 lignes manquantes en mire
            sendLineYUV(-1, NULL, PROG_MEM); //for pattern                
        }
    }
    sendEndVis();
    standby(); // turn off transmitter
    Serial.println(F("done!"));
    //down();
}


bool Sstv::playFmSample() {

    String path = "/f4kmn.raw";
    int cpt = 0;
    int8_t* samp = (int8_t*)ps_malloc(20000);
    
    File file = SDFileSystem.open(path.c_str(), FILE_READ);

    if (!file) {
        Serial.println("Failed to open file in writing mode");
        return false;
    } else {
        while (file.available()) {
            samp[cpt] = file.read();
            cpt++;
        }
        file.close();
        Serial.println(F("read done!"));
        delay(100);  //a diminuer ?
        begin();
        irqDone = 0;
        timerAlarmWrite(timer, 1000, true);
        timerAlarmEnable(timer);
        timerMux = portMUX_INITIALIZER_UNLOCKED;
        for (cpt = 0; cpt < 20000; cpt++) {            
            toneUs(((float) samp[cpt])*40, 125); //Tech=125µs->8000hz     Coef 10000khz / 256 (8 bits) =39.6
        }
        standby(); // turn off transmitter
        
        Serial.println(F("play done!"));
        free(samp);

    }
   

    return true;
}


Sstv* Sstv::anchor = NULL;