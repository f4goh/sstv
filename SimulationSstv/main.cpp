/* 
 * File:   main.cpp
 * Author: ale
 *
 * 
 *  * Created on 22 septembre 2021, 09:39
 * ajouter dans le linker additional Options : -lasound -lm
 * https://www.alsa-project.org/alsa-doc/alsa-lib/pcm.html
 * https://gist.github.com/ghedo/963382/
 * https://soundprogramming.net/programming/alsa-tutorial-1-initialization/
 * https://www.alsa-project.org/alsa-doc/alsa-lib/_2test_2pcm_min_8c-example.html
 * apt-get install libasound2-dev
 * locate asoundlib.h
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iostream>

#include "Sstv.h"

using namespace std;

/*
 * 
 */



int main(int argc, char** argv) {
    /*
     manque une fonctionnalité "arguments"
    */
    
    Sstv monSstv;    
    monSstv.setSampleFreq(48000.0);
    monSstv.setHpEn(true); //active l'option sortie sonore sur HP
    //monSstv.idle();
   monSstv.sendMire("Scottie1.wav", Scottie1);
  //  monSstv.sendBmp("bandes.bmp","Scottie1.wav", Scottie1);
  //  monSstv.sendBmp("montagne320.bmp","Scottie1.wav", Scottie1);
   // monSstv.sendBmp("montagne640.bmp","PasokonP3.wav",PasokonP3);  //a revoir 640
   
   //monSstv.sendMire("Scottie2.wav", Scottie2);
   //monSstv.sendMire("ScottieDX.wav", ScottieDX);
 //  monSstv.sendMire("Martin1.wav", Martin1);
//   monSstv.sendMire("Martin2.wav", Martin2);
  // monSstv.sendMire("PasokonP3.wav", PasokonP3);
   // monSstv.sendMire("PasokonP5.wav", PasokonP5);
   // monSstv.sendMire("PasokonP7.wav", PasokonP7);
    
    return 0;
}

