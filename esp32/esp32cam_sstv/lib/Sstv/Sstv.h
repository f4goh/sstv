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

#include <Arduino.h>
#include <FS.h>     // SD Card ESP32
#include <SD.h> // SD Card ESP32
#include "AD9850SPI.h"
#include "image.h"
#include "mire.h"


#define FREQ 7041000  //40m
//#define  FREQ 14240000  //20m
//#define  FREQ 14260000  //20m

//#define SYNC 13

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
#define SSTV_ROBOT_72                                 12
#define SSTV_ROBOT_36                                 8
#define SSTV_PD_50                                    93
#define SSTV_PD_90                                    99
#define SSTV_MP_73_N                                  2

// SSTV tones in Hz common large txing modes
#define SSTV_TONE_LEADER_L                            1900
#define SSTV_TONE_BREAK_L                             1200
#define SSTV_TONE_VIS_1_L                             1100
#define SSTV_TONE_VIS_0_L                             1300
#define SSTV_TONE_BRIGHTNESS_MIN_L                    1500
#define SSTV_TONE_BRIGHTNESS_MAX_L                    2300

// SSTV tones in Hz narrow txing modes
#define SSTV_TONE_LEADER_N                            2100  // dans qsstv 2100 ok alorq que dans la doc 2300 lent au décodage -> erreur doc !!
#define SSTV_TONE_BREAK_N                             1900
#define SSTV_TONE_VIS_1_N                             1900
#define SSTV_TONE_VIS_0_N                             2100
#define SSTV_TONE_BRIGHTNESS_MIN_N                    2044
#define SSTV_TONE_BRIGHTNESS_MAX_N                    2300


// calibration header timing in us
#define SSTV_HEADER_LEADER_LENGTH                     300000
#define SSTV_HEADER_BREAK_LENGTH                      10000
#define SSTV_HEADER_BIT_LENGTH                        30000
#define SSTV_HEADER_END_LENGTH                        100000
#define SSTV_HEADER_BIT_NARROW_LENGTH                 22000

#define CANAL_GAIN 3

#define CONSIGNE_GAIN 130 //a re mesurer sur 40m
//#define CONSIGNE_GAIN 210 //a revoir 20m

#define PWM_PIN 33


/*!
  \struct tone_t
  \brief Structure to save data about tone.
*/
typedef enum {                //brief Tone type: GENERIC for sync and porch tones, SCAN_GREEN, SCAN_BLUE and SCAN_RED for scan lines.
    GENERIC = 0,
    SCAN_GREEN,
    SCAN_BLUE,
    SCAN_RED,
    SCAN_Y,
    SCAN_Y_EVEN,
    SCAN_Y_ODD,
    SCAN_R_Y,
    SCAN_B_Y            
} toneType;

typedef enum {
    PROG_MEM = 0,
    CAMERA
} imageType;

typedef enum {
    RGB = 0,
    YUV
} modeCoul;

typedef enum {
    LARGE = 0,
    NARROW
} largeNarrow;



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
  tone_t tones[12]; //brief Sequence of tones in each transmission line. This is used to create the correct encoding sequence.
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
extern const SSTVMode_t Robot72;
extern const SSTVMode_t Robot36;
extern const SSTVMode_t PD50;
extern const SSTVMode_t PD90;
extern const SSTVMode_t MP73N;


class Sstv : public AD9850SPI  
{
public:
    Sstv();
    Sstv(const Sstv& orig);
    virtual ~Sstv();
    void tx(const SSTVMode_t &_mode);
    void idle();
    void toneUs(float freq, uint32_t len);
    void modPWM(uint8_t _pwmValue, uint32_t len);
    
    void sendHeaderNarrow(); //test
    void sendLineYUVNarrow(int idxLine,uint8_t *ptr,imageType imgtype); //test
    void sendMireNarrow();  //test
    void sendImgNarrow(); //test
    void sendCameraYUVNarrow(uint8_t *ptr); //test
    
    
    void sendHeaderStandard(); 
    void sendEndVis();
    void sendLineRGB(int idxLine,uint8_t *ptr,imageType imgtype);
    void sendLineYUV(int idxLine,uint8_t *ptr,imageType imgtype);
    void sendLineCamera(uint8_t *ptr);
    void sendCameraYUV(uint8_t *ptr);
    void standby();
    void sendMire(modeCoul mCoul);
    void sendImg(modeCoul mCoul);
    void sendCameraRGB(uint8_t *ptr);
    bool playFmSample();
    bool playAMSample();
    void addGain(int8_t deltaGain);
    
    
    SSTVMode_t mode;
private:
    hw_timer_t * timer;
    void IRAM_ATTR interuption();
    static void marshall();
    portMUX_TYPE timerMux;
        
    static Sstv* anchor;
    int nbEchPerBit;    //nombre d'échantillons pour un bit en accord avec le bit rate
    bool firstLine;
    volatile bool irqDone;
    volatile long frequency;
    volatile long tPeriod;
    volatile uint8_t pwmValue; //gain sur le pa

};

#endif /* SSTV_H */

