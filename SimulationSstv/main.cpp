/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: ale
 *
 * 
 *  * Created on 22 septembre 2021, 09:39
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

