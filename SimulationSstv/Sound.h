/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   Sound.h
 * Author: alecren
 *
 * Created on 19 juin 2022, 14:48
 */

#ifndef SOUND_H
#define SOUND_H

#include <alsa/asoundlib.h>
#include <math.h> 

#define BUF_SIZE 16368

class Sound {
public:
    Sound();
    Sound(const Sound& orig);
    virtual ~Sound();
    
    void begin();
    void play();
    void load();
    void close();
    void loadAndPlay(int16_t val);
    void end();
    
    
    
private:
char device[8];
//unsigned char buffer[16*1024];              /* some random data */
//unsigned char buffer[BUF_SIZE];
int16_t buffer[BUF_SIZE];
snd_pcm_t *handle;
snd_pcm_sframes_t frames;
int ptr;

};

#endif /* SOUND_H */

