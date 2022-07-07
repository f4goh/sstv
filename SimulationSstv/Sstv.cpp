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
sampleFreq=SAMPLING_FREQUENCY;
rapport=DDS_SAMPLING_FREQUENCY/SAMPLING_FREQUENCY;
soundout = new Sound();
hpEn=false;
}

Sstv::Sstv(const Sstv& orig) {
}

Sstv::~Sstv() {
    delete soundout;
}

void Sstv::setSampleFreq(float _sampleFreq)
{
  sampleFreq=_sampleFreq;
  rapport=DDS_SAMPLING_FREQUENCY/sampleFreq;
}

void Sstv::idle() {
  Dds::incrementPhase = Dds::computeIncrementPhase(SSTV_TONE_LEADER);   
  int n;
  for (n=0;n<100;n++){
     cout << static_cast<int>(Dds::simulationInteruption()) << endl; 
  }
}
/*
 * ancienne version qui sera compatible avec l'esp32
void Sstv::tone(float freq, uint32_t len) {
    
   uint32_t t;
   uint8_t downSampling;
    Dds::incrementPhase = Dds::computeIncrementPhase(freq);
        for (t = 0; t < len; t++) {
            downSampling = Dds::simulationInteruption();            
            if (Dds::compteur==DOWNSAMPLING){                
                fichier.write((char *) &downSampling, sizeof (downSampling));
                Dds::raz();
            }
        }
}
*/

/*
  manque l'interpolation pour le changement de fréquence (non critique)
 */
void Sstv::tone(float freq, uint32_t len) {
   
   uint32_t t;
   uint8_t downSampling;
   //char z=0;
   int16_t sample;
    Dds::incrementPhase = Dds::computeIncrementPhase(freq);
        for (t = 0; t < len; t++) {
            downSampling = Dds::simulationInteruption();            
            if (Dds::compteur==idx){
                //write_word(fichier,((static_cast<int>(downSampling))<<7)-16384, 2 );
                sample=((static_cast<int16_t>(downSampling))<<7)-16384;
                 if (hpEn){
                  soundout->loadAndPlay(sample);
                 }
                fichier.write(reinterpret_cast<char*>(&sample), sizeof (int16_t));               
                offset-=idx;
                offset+=rapport;
                idx=static_cast<uint32_t>(offset);
                Dds::raz();
            }
        }
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


void Sstv::initWave(wave *wav){
    
}


void Sstv::sendMire(string ficName,const SSTVMode_t &_mode) {
    
    uint32_t line[320*2] = {
    // black
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,

    // blue
    0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF,
    0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF,

    // green
    0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00,
    0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00,

    // cyan
    0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF,
    0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF,

    // red
    0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000,
    0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000,

    // magenta
    0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF,
    0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF,

    // yellow
    0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00,
    0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00,

    // white
    0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF,
    0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF,
   
      // black
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,

    // blue
    0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF,
    0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF,

    // green
    0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00,
    0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00,

    // cyan
    0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF,
    0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF,

    // red
    0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000,
    0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000,

    // magenta
    0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF,
    0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF,

    // yellow
    0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00,
    0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00,

    // white
    0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF,
    0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF
};
    
    if (hpEn) soundout->begin();
  
    
    
    mode = _mode;    
    fichier.open(ficName, fstream::binary | fstream::out);
 
    if (!fichier.is_open())
        cerr << "Erreur lors de l'ouverture du fichier" << endl;
    else {
        //https://openclassrooms.com/forum/sujet/lecture-de-fichiers-wav-en-c
        wave wav = {0};
         
        /* Les constantes symboliques */
        strncpy(wav.riff, "RIFF", 4);
        strncpy(wav.wave, "WAVE", 4);
        strncpy(wav.fmt, "fmt ", 4);
        strncpy(wav.Ndata, "data", 4);

        //wav->taille = wav->subTaille2 = 0;

        wav.subTaille1 = T_PCM;
        wav.formatAudio = PCM;
        wav.nombreCanaux = MONO;
        wav.freqEch = static_cast<int32_t>(sampleFreq);
        wav.bitsParEch = BPE_16;
        wav.ByteRate = wav.freqEch * wav.nombreCanaux * (wav.bitsParEch / 8);
        wav.align = wav.nombreCanaux * (wav.bitsParEch / 8);

        fichier.write((char*) &wav, sizeof (wav)); //ecrit l'entete du fichier wav
        
        cout << "[SSTV] write " << ficName << " mire " << endl;   
        offset=rapport;
        cout << "Offset downsampling " << offset <<endl;;       
        idx=static_cast<uint32_t>(offset);
        Dds::raz();
        
        sendHeader(); // send synchronization header first
        
        for (uint16_t i = 0; i < getPictureHeight(); i++) {    
            sendLine(line);
        }
        
        size_t file_length = fichier.tellp();   //recupere la taille des données écites
        wav.subTaille2 = file_length-44;    //positionne les tailles des donnes écrites dans l'entete wav
        wav.taille = file_length-8;         //idem avec la taille globale
                
        fichier.seekp(0);       //retour au début du fichier
        fichier.write((char*) &wav, sizeof (wav)); //réecrit l'en-tete
        
        
        fichier.close();
        if (!fichier.good()) {
            cout << "Error occurred at writing time!" << endl;            
        }
        cout << "done!";
        if (hpEn){
            soundout->end();
            soundout->close();
        }
    }
    
}



void Sstv::sendBmp(string ficBmp,string ficName,const SSTVMode_t &_mode) {
    
    mode = _mode;    
    fichier.open(ficName, fstream::binary | fstream::out);
 
    if (!fichier.is_open())
        cerr << "Erreur lors de l'ouverture du fichier" << endl;
    else {
        //https://openclassrooms.com/forum/sujet/lecture-de-fichiers-wav-en-c
        wave wav = {0};
         
        /* Les constantes symboliques */
        strncpy(wav.riff, "RIFF", 4);
        strncpy(wav.wave, "WAVE", 4);
        strncpy(wav.fmt, "fmt ", 4);
        strncpy(wav.Ndata, "data", 4);

        //wav->taille = wav->subTaille2 = 0;

        wav.subTaille1 = T_PCM;
        wav.formatAudio = PCM;
        wav.nombreCanaux = MONO;
        wav.freqEch = static_cast<int32_t>(sampleFreq);
        wav.bitsParEch = BPE_16;
        wav.ByteRate = wav.freqEch * wav.nombreCanaux * (wav.bitsParEch / 8);
        wav.align = wav.nombreCanaux * (wav.bitsParEch / 8);

        fichier.write((char*) &wav, sizeof (wav)); //ecrit l'entete du fichier wav
        
        cout << "[SSTV] write " << ficName << " mire " << endl;   
        offset=rapport;
        cout << "Offset downsampling " << offset <<endl;;       
        idx=static_cast<uint32_t>(offset);
        Dds::raz();
 
          if (hpEn) soundout->begin();
 
        sendHeader(); // send synchronization header first
        
        
        fichierImage.open(ficBmp, fstream::binary | fstream::in);
        if (!fichierImage.is_open())
        cerr << "Erreur lors de l'ouverture du fichier" << endl;
    else {
        enTeteFichierBmp fichierBmp;
        enTeteImageBmp imageBmp;
        
        fichierImage.read((char*) &fichierBmp, sizeof (enTeteFichierBmp));
        fichierImage.read((char*) &imageBmp, sizeof (enTeteImageBmp));
        
        afficheEnTeteFichierBmp(fichierBmp);
        afficheEnTeteImageBmp(imageBmp);
    
        uint8_t pixels[imageBmp.tailleImage];    
        fichierImage.read((char*) &pixels, imageBmp.tailleImage);

    
        uint32_t line[640]; //320

        int n;
        uint32_t coul;
        int ptr,index;
        //ptr=(getPictureHeight()-1)*320*3;
        ptr=(getPictureHeight()-1)*640*3;
        index=0;
        for (uint16_t i = 0; i < getPictureHeight(); i++) {
            for (n = ptr; n < ptr+(640 * 3); n += 3) { //320
                coul = pixels[n + 2]*256 * 256 + pixels[n + 1]*256 + pixels[n];
                //cout << hex << coul << endl;
                line[index++]=coul;
            }
            index=0;
            ptr-=640*3; //320
            sendLine(line);
        }
    
    }
        size_t file_length = fichier.tellp();   //recupere la taille des données écites
        wav.subTaille2 = file_length-44;    //positionne les tailles des donnes écrites dans l'entete wav
        wav.taille = file_length-8;         //idem avec la taille globale
                
        fichier.seekp(0);       //retour au début du fichier
        fichier.write((char*) &wav, sizeof (wav)); //réecrit l'en-tete
        
        fichierImage.close();
        
        fichier.close();
        if (!fichier.good()) {
            cout << "Error occurred at writing time!" << endl;            
        }
        cout << "done!";
          if (hpEn){
            soundout->end();
            soundout->close();              
          }
    }
    
}



void Sstv::afficheEnTeteFichierBmp(enTeteFichierBmp fichierBmp)
{
   cout <<  fichierBmp.signature[0] << fichierBmp.signature[1] << endl;
   cout <<  fichierBmp.tailleFichier << endl;
   cout <<  fichierBmp.adresseImg << endl;
}

void Sstv::afficheEnTeteImageBmp(enTeteImageBmp imageBmp)
{
  cout <<  imageBmp.tailleEnTete << endl;
  cout << "tailleEnTete " << imageBmp.tailleEnTete<< endl;
  cout << "largeur " << imageBmp.largeur<< endl;
  cout << "hauteur " << imageBmp.hauteur<< endl;
  cout << "nbPlan " << imageBmp.nbPlan<< endl;
  cout << "bitParPixel " << imageBmp.bitParPixel<< endl;
  cout << "typeCompression " << imageBmp.typeCompression<< endl;
  cout << "tailleImage " << imageBmp.tailleImage<< endl;
  cout << "resolutionHorizontale " << imageBmp.resolutionHorizontale<< endl;
  cout << "resolutionVerticale "<< imageBmp.resolutionVerticale<< endl;
  cout << "nbCouleursUtilises " << imageBmp.nbCouleursUtilises<< endl;
  cout << "nbCouleursImportantes "<< imageBmp.nbCouleursImportantes<< endl;
}

void Sstv::setHpEn(bool _hpEn){
 hpEn=_hpEn;   
}