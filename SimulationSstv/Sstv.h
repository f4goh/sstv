/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Sstv.h
 * Author: ale
 *
 * Created on 19 septembre 2021, 11:50
 * d'apès
 * https://github.com/jgromes/RadioLib/tree/master/src/protocols/SSTV
 */

#ifndef SSTV_H
#define SSTV_H

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iostream>
#include <cstring>
   
           
#include "Dds.h"
#include "wav.h"
#include "bitmap.h"

using namespace std;


// VIS codes
#define SSTV_SCOTTIE_1                                60
#define SSTV_SCOTTIE_2                                56
#define SSTV_SCOTTIE_DX                               76
#define SSTV_MARTIN_1                                 44
#define SSTV_MARTIN_2                                 40
#define SSTV_WRASSE_SC2_180                           55
#define SSTV_PASOKON_P3                               113
#define SSTV_PASOKON_P5                               114
#define SSTV_PASOKON_P7                               115

// SSTV tones in Hz
#define SSTV_TONE_LEADER                              1900
#define SSTV_TONE_BREAK                               1200
#define SSTV_TONE_VIS_1                               1100
#define SSTV_TONE_VIS_0                               1300
#define SSTV_TONE_BRIGHTNESS_MIN                      1500
#define SSTV_TONE_BRIGHTNESS_MAX                      2300

// calibration header timing in us
#define SSTV_HEADER_LEADER_LENGTH                     300000
#define SSTV_HEADER_BREAK_LENGTH                      10000
#define SSTV_HEADER_BIT_LENGTH                        30000

//#define DOWNSAMPLING DDS_SAMPLING_FREQUENCY/25000
#define SAMPLING_FREQUENCY 48000.0

/*!
  \struct tone_t
  \brief Structure to save data about tone.
*/
typedef enum {                //brief Tone type: GENERIC for sync and porch tones, SCAN_GREEN, SCAN_BLUE and SCAN_RED for scan lines.
    GENERIC = 0,
    SCAN_GREEN,
    SCAN_BLUE,
    SCAN_RED
} toneType;


typedef struct  {
  toneType tt;
  uint32_t len; //brief Length of tone in us, set to 0 for picture scan tones.
  uint16_t freq; // brief Frequency of tone in Hz, set to 0 for picture scan tones.
}tone_t;

/*!
  \struct SSTVMode_t
  \brief Structure to save data about supported SSTV modes.
*/
typedef struct  {
  uint8_t visCode; //brief Unique VIS code of the SSTV mode.
  uint16_t width; //brief Picture width in pixels.
  uint16_t height; //brief Picture height in pixels.
  uint16_t scanPixelLen; //brief Pixel scan length in us.
  uint8_t numTones; //brief Number of tones in each transmission line. Picture scan data is considered single tone.
  tone_t tones[8]; //brief Sequence of tones in each transmission line. This is used to create the correct encoding sequence.
}SSTVMode_t;

extern const SSTVMode_t Scottie1;
extern const SSTVMode_t Scottie2;
extern const SSTVMode_t ScottieDX;
extern const SSTVMode_t Martin1;
extern const SSTVMode_t Martin2;
extern const SSTVMode_t Wrasse;
extern const SSTVMode_t PasokonP3;
extern const SSTVMode_t PasokonP5;
extern const SSTVMode_t PasokonP7;



class Sstv : public Dds 
{
public:
    Sstv();
    Sstv(const Sstv& orig);
    virtual ~Sstv();
    void setSampleFreq(float sampleFreq);
    void idle();
    void initWave(wave *wav);
    void sendMire(const string ficName,const SSTVMode_t &_mode);
    void sendBmp(string ficBmp,string ficName,const SSTVMode_t &_mode);
    
private:
    uint16_t getPictureHeight();  
    void tone(float freq, uint32_t len);
    void sendHeader();
    void sendLine(uint32_t* imgLine);
    void afficheEnTeteFichierBmp(enTeteFichierBmp fichierBmp);
    void afficheEnTeteImageBmp(enTeteImageBmp imageBmp);
    
    SSTVMode_t mode;
    fstream fichier;
    fstream fichierImage;
    
    int nbEchPerBit;    //nombre d'échantillons pour un bit en accord avec le bit rate
    bool firstLine;
    float offset;
    float rapport;
    uint32_t idx;
    float sampleFreq;
    
};

#endif /* SSTV_H */

