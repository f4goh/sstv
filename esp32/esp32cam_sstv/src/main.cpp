/* 
 * File:   main.cpp
 * Author: philippe SIMIER Touchard washington Le Mans
           Anthony LE CREN F4GOH KF4GOH Touchard washington Le Mans
 *
 * Created on 25 juin 2022, 18:05
 * FRAMESIZE_QVGA  // 320 x 240
 * FRAMESIZE_VGA   // 640x480
 * 40 m : 7,033-7,040 7.171Mhz
 */


#include <Arduino.h>
#include <Camera.h>
#include "Sstv.h"
#include "SSTVDisplay.h"



SSTVMode_t mode=Scottie1; //defaut mode

Camera *laCamera;
Sstv monSstv;
SSTVDisplay incrustation; // a modifier ou mettre des setters pour la taille de l'image
                          //pour le moment test 320 pixels de large fixe (voir constructeur)




void setup() {
    Serial.begin(115200);

   
    laCamera = new Camera;

       if (!laCamera->init(PIXFORMAT_JPEG, FRAMESIZE_VGA)) {
        Serial.println("Camera init failed");
        while (1) {
            delay(1000);
        }
    }
    
    if (!laCamera->SDinit()) {
        Serial.println("SD card init failed");
        while (1) {
            delay(1000);
        }
    }
    
    //Serial.println("I2C scanner");
    //incrustation.I2Ctest();
    
    Serial.printf("Internal Total heap %d, internal Free Heap %d\n", ESP.getHeapSize(), ESP.getFreeHeap());
    Serial.printf("SPIRam Total heap %d, SPIRam Free Heap %d\n", ESP.getPsramSize(), ESP.getFreePsram());
  
    
    Serial.println("Setup done");
    //digitalWrite(LED_ROUGE, HIGH);
    
    Serial.println("\n\n1 Mode Scottie1");
    Serial.println("2 Mode Scottie2");
    Serial.println("3 Mode ScottieDX");
    Serial.println("4 Mode Martin1");
    Serial.println("5 Mode Martin2");
    Serial.println("6 Mode Wrasse");
    Serial.println("7 Mode PasokonP3");
    Serial.println("8 Mode PasokonP5");
    Serial.println("9 Mode PasokonP7");    
    Serial.println("a Mode Robot36 YUV");    
    Serial.println("b Mode Robot72 YUV");    
    Serial.println("c Mode PD50 YUV");    
    Serial.println("d Mode PD90 YUV");    
    Serial.println("e Mode MP73-N YUV");
    Serial.println("f Add position from arduino pro mini");   
    
    Serial.println("k tx picture with camera");
    Serial.println("m tx mire RGB or YUV");
    Serial.println("p tx test picture in progmem");
    Serial.println("r save picture on msd card");
    Serial.println("i tx idle frequency tone break 1200Hz");
    Serial.println("s stop idle\n");
    Serial.println("t test FM Narrow Sample");
    Serial.println("u test AM Sample");    
    Serial.println("v test USB Sample");
    Serial.println("w test LSB Sample");

    
    Serial.println("+ increase gain by +10");
    Serial.println("- decrease gain by -10");
    
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
            case 'a':
                mode = Robot36;
                Serial.println("Mode Robot36");
                break;
            case 'b':
                mode = Robot72;
                Serial.println("Mode Robot72");
                break;
            case 'c':
                mode = PD50;
                Serial.println("Mode PD50");
                break;
            case 'd':
                mode = PD90;
                Serial.println("Mode PD90");
                break;
            case 'e':
                mode = MP73N;
                Serial.println("Mode MP73-N");                
                break;
            case 'f':
                incrustation.addPosition();
                break;

            case 'k':
                Serial.print("FREE HEAP :");
                Serial.println(ESP.getFreeHeap());
                laCamera->reset(10);
                if (mode.visCode == SSTV_ROBOT_36 || mode.visCode == SSTV_ROBOT_72 || mode.visCode == SSTV_PD_50 || mode.visCode == SSTV_PD_90 || mode.visCode == SSTV_MP_73_N) {
                    Serial.println("YUV565 QVGA");
                    laCamera->init(PIXFORMAT_YUV422, FRAMESIZE_QVGA);
                    laCamera->capturePhoto();
                    incrustation.drawString(100, 30, "F4KMN", laCamera->getBuf(), YUV);
                    monSstv.tx(mode);
                    if (mode.visCode == SSTV_MP_73_N) {
                        monSstv.sendCameraYUVNarrow(laCamera->getBuf());
                    } else {
                        monSstv.sendCameraYUV(laCamera->getBuf());
                    }                    
                } else {
                    Serial.println("RGB565 QVGA");
                    laCamera->init(PIXFORMAT_RGB565, FRAMESIZE_QVGA);
                    laCamera->capturePhoto();
                    incrustation.drawString(100, 30, "F4KMN", laCamera->getBuf(), RGB);                    
                    monSstv.tx(mode);
                    monSstv.sendCameraRGB(laCamera->getBuf());                
                }
                laCamera->frameback();
                break;
            case 'm':
                Serial.print("FREE HEAP :");
                Serial.println(ESP.getFreeHeap());               
                monSstv.tx(mode);
                if (mode.visCode == SSTV_ROBOT_36 || mode.visCode == SSTV_ROBOT_72 || mode.visCode == SSTV_PD_50 || mode.visCode == SSTV_PD_90) {
                    monSstv.sendMire(YUV);
                } else if (mode.visCode == SSTV_MP_73_N) {
                    monSstv.sendMireNarrow();
                } else {
                    monSstv.sendMire(RGB);
                }                
                break;
            case 'p':
                Serial.print("FREE HEAP :");
                Serial.println(ESP.getFreeHeap());                
                monSstv.tx(mode);
                if (mode.visCode == SSTV_ROBOT_36 || mode.visCode == SSTV_ROBOT_72 || mode.visCode == SSTV_PD_50 || mode.visCode == SSTV_PD_90) {
                    monSstv.sendImg(YUV);
                } else if (mode.visCode == SSTV_MP_73_N) {
                    monSstv.sendImgNarrow();
                } else {
                    monSstv.sendImg(RGB);
                }       
                break;
            case 'r':
                Serial.print("FREE HEAP :");
                Serial.println(ESP.getFreeHeap());
                laCamera->reset(10);
                laCamera->init(PIXFORMAT_JPEG, FRAMESIZE_VGA);
                Serial.println("JPEG VGA");
                if (laCamera->capturePhoto()) {
                    laCamera->SaveSD("photo");
                    Serial.printf("len  : %d octets \n\r", laCamera->getLen());
                    Serial.printf("size : %d x %d \n\r", laCamera->getwidth(), laCamera->getheight());                    
                }
                break;
            case 'i':
                Serial.println("test idle");
                //ledcWrite(CANAL_GAIN, gain);
                monSstv.idle();
                break;
            case 's':
                Serial.println("standby an gain off");
                monSstv.standby();                                
                break;
             case 't':
                Serial.println("Play FM Narrow sample 8bit Fech 8000Hz");
                monSstv.playFmSample();
                break;
             case 'u':
                Serial.println("Play AM Narrow sample 8bit Fech 8000Hz");
                monSstv.playAMSample();
                break;
             case 'v':
                Serial.println("Play SSB USB sample 8bit Fech 8000Hz");
                monSstv.playSSBSample(USB);                
                break;
             case 'w':
                Serial.println("Play SSB LSB sample 8bit Fech 8000Hz");
                monSstv.playSSBSample(LSB);                
                break;
            case '+':
                monSstv.addGain(10);
                break;
            case '-':
                monSstv.addGain(-10);
                break;
        }
    }
}
