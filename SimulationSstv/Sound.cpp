/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   Sound.cpp
 * Author: alecren
 * 
 * Created on 19 juin 2022, 14:48
 * SND_PCM_FORMAT_U8 	

Unsigned 8 bit
SND_PCM_FORMAT_S16_LE 	

Signed 16 bit Little Endian
SND_PCM_FORMAT_S16_BE 	

Signed 16 bit Big Endian
SND_PCM_FORMAT_U16_LE 	

Unsigned 16 bit Little Endian
SND_PCM_FORMAT_U16_BE 	

Unsigned 16 bit Big Endian 
 * 
 */

#include "Sound.h"

Sound::Sound() :
device{"default"},
ptr(0)
{
}

Sound::Sound(const Sound& orig) {
}

Sound::~Sound() {
}

void Sound::begin() {
    int err;

    if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        printf("Playback open error: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }

    if ((err = snd_pcm_set_params(handle,
            SND_PCM_FORMAT_S16_LE, // ou SND_PCM_FORMAT_S16_BE
            SND_PCM_ACCESS_RW_INTERLEAVED,
            1, //channels
            48000, //sample rate
            1, //soft_resample,
            500000)) < 0) { // 0.5sec latency
        printf("Playback open error: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }
}

void Sound::loadAndPlay(int16_t val){
    buffer[ptr++]=val;
    if (ptr==BUF_SIZE){
        ptr=0;
        frames = snd_pcm_writei(handle, buffer, BUF_SIZE);
    }
}

void Sound::end(){
    int i;
    while(ptr<BUF_SIZE){
        buffer[ptr++]=0;
    }
    ptr=0;
    frames = snd_pcm_writei(handle, buffer, BUF_SIZE);
}

    

void Sound::load(){
    int i;
       for (i = 0; i < BUF_SIZE; i++){
        buffer[i] =  10000* sin (i*2*M_PI/48);  //2*pi -> 48
        //printf("%d,",buffer[i]);
    }
}

void Sound::play()
{
    int i,j;
     for (j = 0; j < 4; j++) {
        frames = snd_pcm_writei(handle, buffer, BUF_SIZE);
    
       // if (frames < 0)
       //     frames = snd_pcm_recover(handle, frames, 0);
        /*
        if (frames < 0) {
            printf("snd_pcm_writei failed: %s\n", snd_strerror(frames));
            break;
        }
        if (frames > 0 && frames < (long)BUF_SIZE)
            printf("Short write (expected %li, wrote %li)\n", (long)BUF_SIZE, frames);
         **/
    }
}

void Sound::close(){
   // pass the remaining samples, otherwise they're dropped in close
    int err;
    err = snd_pcm_drain(handle);
    if (err < 0)
        printf("snd_pcm_drain failed: %s\n", snd_strerror(err));
    snd_pcm_close(handle); 
}