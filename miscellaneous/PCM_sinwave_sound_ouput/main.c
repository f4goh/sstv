/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: ale
 *
 * Created on 6 octobre 2021, 19:42
 * ajouter dans le linker additional Options : -lasound
 * https://www.alsa-project.org/alsa-doc/alsa-lib/pcm.html
 * https://gist.github.com/ghedo/963382/
 * https://soundprogramming.net/programming/alsa-tutorial-1-initialization/
 * https://www.alsa-project.org/alsa-doc/alsa-lib/_2test_2pcm_min_8c-example.html
 * apt-get install libasound2-dev
 * locate asoundlib.h
 * voir fichier sound.h avec le #include <alsa/asoundlib.h>
 */

#include <stdio.h>
#include <stdlib.h>
/*
 *  This extra small demo sends a random samples to your speakers.
 */
#include <math.h> 
#include <alsa/asoundlib.h>
static char *device = "default";            /* playback device */
//unsigned char buffer[16*1024];              /* some random data */
unsigned char buffer[16368]; //
//int buffer[16*1024];              /* some random data */

/*
 * 
 */
int main(int argc, char** argv) {
    int err;
    unsigned int i,j;
    snd_pcm_t *handle;
    snd_pcm_sframes_t frames;
 
    for (i = 0; i < sizeof(buffer); i++){
        //buffer[i] = random() & 0xffff;
        buffer[i] = 128 + 127* sin (i*2*M_PI/48);  //2*pi -> 48
        //printf("%d,",buffer[i]);
    }
        
 
    
    if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        printf("Playback open error: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }

    if ((err = snd_pcm_set_params(handle,
                      SND_PCM_FORMAT_U8, //SND_PCM_FORMAT_S16_BE
                      SND_PCM_ACCESS_RW_INTERLEAVED,
                      1,  //channels
                      48000, //sample rate
                      1, //soft_resample,
                      500000)) < 0) {   // 0.5sec latency
        printf("Playback open error: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }

    for (j = 0; j < 16; j++) {
        frames = snd_pcm_writei(handle, buffer, sizeof (buffer));
        for (i = 0; i < sizeof (buffer); i++) {
            //buffer[i] = random() & 0xffff;
            buffer[i] = 128 + 127 * sin(i * 2 * M_PI / 48); //2*pi -> 48
            //printf("%d,",buffer[i]);
        }

        if (frames < 0)
            frames = snd_pcm_recover(handle, frames, 0);
        if (frames < 0) {
            printf("snd_pcm_writei failed: %s\n", snd_strerror(frames));
            break;
        }
        if (frames > 0 && frames < (long)sizeof(buffer))
            printf("Short write (expected %li, wrote %li)\n", (long)sizeof(buffer), frames);
    }
 
    // pass the remaining samples, otherwise they're dropped in close
    err = snd_pcm_drain(handle);
    if (err < 0)
        printf("snd_pcm_drain failed: %s\n", snd_strerror(err));
    snd_pcm_close(handle);
    
    return (EXIT_SUCCESS);
}

