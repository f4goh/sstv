#include <Arduino.h>
#include "Sstv.h"


Sstv monSstv;

SSTVMode_t mode=Scottie1; //defaut mode


void setup() {
    Serial.begin(115200);
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
                Serial.println("Mode PasokonP3");                
                break;
            case '8':
                mode=PasokonP5;                
                Serial.println("Mode PasokonP5");
                break;
            case '9':
                mode=PasokonP7;
                Serial.println("Mode PasokonP7");
                break;
            case 'm':
                monSstv.tx(mode);
                monSstv.sendMire();
                break;
            case 'p':
                monSstv.tx(mode);
                monSstv.sendImg();
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