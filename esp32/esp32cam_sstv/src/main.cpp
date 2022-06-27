/* 
 * File:   main.cpp
 * Author: philippe SIMIER Touchard washington Le Mans
 *
 * Created on 25 juin 2022, 18:05
 */


#include <Arduino.h>
#include <Camera.h>
#include "Sstv.h"


Sstv monSstv;

SSTVMode_t mode=Scottie1; //defaut mode


Camera *laCamera;

void setup() {
    Serial.begin(115200);

    
    pinMode(LED_ROUGE, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(16, OUTPUT); 
    
    digitalWrite(LED_ROUGE, LOW);
    
    
    laCamera = new Camera;

    //  FRAMESIZE_QVGA  // 320 x 240
    //  FRAMESIZE_VGA   // 640x480
    
    
    if (!laCamera->init(PIXFORMAT_RGB565, FRAMESIZE_QVGA)) {
        Serial.println("Problème Camera ou carte SD");
        while (1) {
            delay(1000);
        }
    }
    Serial.println("Setup done");
    digitalWrite(LED_ROUGE, HIGH);
    
    Serial.println("\n\n1 Mode Scottie1");
    Serial.println("2 Mode Scottie2");
    Serial.println("3 Mode ScottieDX");
    Serial.println("4 Mode Martin1");
    Serial.println("5 Mode Martin2");
    Serial.println("6 Mode Wrasse");
    Serial.println("7 Mode PasokonP3");
    Serial.println("8 Mode PasokonP5");
    Serial.println("9 Mode PasokonP7");
    Serial.println("7 Mode PasokonP3");
    Serial.println("c tx picture with camera");
    Serial.println("m tx mire");
    Serial.println("p tx test picture in progmem");
    Serial.println("i tx idle frequency tone break 1200Hz");
    Serial.println("s stop idle\n");
    
}

void loop() {
    char c;
    if (Serial.available() > 0) {
        c = Serial.read();
        switch (c) {
            case '1':
                mode=Scottie1;
                Serial.println("Mode Scottie1");
                break;
            case '2':
                mode=Scottie2;
                Serial.println("Mode Scottie2");
                break;
            case '3':
                mode=ScottieDX;
                Serial.println("Mode ScottieDX");
                break;
            case '4':
                mode=Martin1;
                Serial.println("Mode Martin1");
                break;
            case '5':
                mode=Martin2;
                Serial.println("Mode Martin2");
                break;
            case '6':
                mode=Wrasse;
                Serial.println("Mode Wrasse");
                break;
            case '7':
                mode=PasokonP3;
                Serial.println("Mode PasokonP3");    //non supporté par la caméra         
                break;
            case '8':
                mode=PasokonP5;                
                Serial.println("Mode PasokonP5");  //non supporté par la caméra         
                break;
            case '9':
                mode=PasokonP7;
                Serial.println("Mode PasokonP7");  //non supporté par la caméra             
                break;
            case 'm':
                monSstv.tx(mode);
                monSstv.sendMire();
                break;
            case 'p':
                monSstv.tx(mode);
                monSstv.sendImg();                
                break;
            case 'c':
                laCamera->capturePhotoSstv();                
                monSstv.tx(mode);
                monSstv.sendCamera(laCamera->getBuf());                
                break;                
            case 'i':
                Serial.println("test idle");
                monSstv.idle();
                break;
            case 's':
                Serial.println("standby");
                monSstv.standby();
                break;
        }
    }
}
    
    
    /*
    digitalWrite(LED_ROUGE, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(16, HIGH);
    delay(500);
    laCamera->capturePhotoSaveSD("photo");
    digitalWrite(LED_ROUGE, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(16, LOW);
    delay(2000);
    */
    //laCamera->flash(1);

