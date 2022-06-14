/* Author: ale
 * 
 * Created on 19 septembre 2021, 11:50
 * 
 * SSTV Transmit AFSK Example
 * Adaptation sur l'esp32 avec le dds sous IRQ timer
 * décodage avec qsstv sur PC: sudo apt install qsstv
 * Comment démarrer ?
 * Programmer l'esp32 (compatible carte ruche)
 * dans qsstv  (sudo apt install qsstv)
 * Configurer la carte son (voir qsstv config carte son.png)
 * activer la réception dans qsstv (triangle de lecture)
 * puis envoyer une porteuse 1900Hz (touche i sur le moniteur série)
 * vérifier la porteuse dans le waterfall de qsstv
 * stopper la porteuse (touche s sur le moniteur série)
 * envoyer une mire (touche 1 à 9) sauf touche 6 mode inconnu pour le moment (sstv_mire_scottie1.png)
 * il se peut que le décodeur qsstv bloque il faut relancer la réception (triangle de lecture)
 * 
 * documentation : sstv.pdf
 * et lien téléchargeable
 * https://www.sstv-handbook.com/download/sstv-handbook.pdf
 * 
 *
 * 
 */

#include <Arduino.h>
#include "Sstv.h"

Sstv monSstv;


// test "image" - actually just a single 320px line
// will be sent over and over again, to create vertical color stripes at the receiver
uint32_t line[320] = {
    // black
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,

    // blue
    0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF,
    0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF,

    // green
    0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00,
    0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00,

    // cyan
    0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF,
    0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF, 0x00FFFF,

    // red
    0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000,
    0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000,

    // magenta
    0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF,
    0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF,

    // yellow
    0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00,
    0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00, 0xFFFF00,

    // white
    0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF,
    0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF
};

float coef=0.98;

void setup() {
    Serial.begin(115200);    
    // SSTV mode:                   Wrasse (SC2-180)
    // correction factor:           0.95
    // NOTE: Due to different speeds of various platforms
    //       supported by RadioLib (Arduino Uno, ESP32 etc),
    //       and because SSTV is analog protocol, incorrect
    //       timing of pulses can lead to distortions.
    //       To compensate, correction factor can be used
    //       to adjust the length of timing pulses
    //       (lower number = shorter pulses).
    //       The value is usually around 0.95 (95%).
}

void sendMire() {
    Serial.println(F("[SSTV] Sending test picture ... "));
    monSstv.sendHeader(); // send synchronization header first
    // send all picture lines
    for (uint16_t i = 0; i < monSstv.getPictureHeight(); i++) {    
        monSstv.sendLine(line);
    }
    monSstv.standby(); // turn off transmitter
    Serial.println(F("done!"));
}



void loop() {
    char c;
    if (Serial.available() > 0) {
        c = Serial.read();
        switch (c) {
            case '1':
                monSstv.begin(Scottie1, 0.98); //ok
                sendMire();
                break;
            case '2':
                monSstv.begin(Scottie2, 0.97); //ok
                sendMire();
                break;
            case '3':
                monSstv.begin(ScottieDX, 0.99); //ok
                sendMire();
                break;
            case '4':
                monSstv.begin(Martin1, 0.984);  //ok
                sendMire();
                break;
            case '5':
                monSstv.begin(Martin2, 0.969);   //ok
                sendMire();
                break;
            case '6':
                monSstv.begin(Wrasse, coef); //coef non trouvé connais pas le mode...
                sendMire();
                break;
            case '7':
                monSstv.begin(PasokonP3, 0.972); //ok
                sendMire();
                break;
            case '8':
                monSstv.begin(PasokonP5, 0.98); //ok
                sendMire();
                break;
            case '9':
                monSstv.begin(PasokonP7, 0.984); //ok
                sendMire();
                break;
            case 'i':
                Serial.println("test idle");
                monSstv.idle();
                break;
            case 's':
                Serial.println("standby");
                monSstv.standby();
                break;
            case '-':
                Serial.println("debug offset pixel decrementation");
                coef -= 0.002;
                monSstv.begin(Scottie1, coef);
                Serial.println(coef,3);
                break;
            case '+':
                Serial.println("debug offset pixel incrementation");
                coef += 0.002;
                monSstv.begin(Scottie1, coef);
                Serial.println(coef,3);
                break;

        }
    }
}