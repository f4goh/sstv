/* 
 * File:   Dds.cpp
 * Author: Anthony (f4goh@orange.fr) & Philippe S (philaure@wanadoo.fr)
 * 
 *  Created on 21 juillet 2021, 18:47
 * La fréquence de la sinusoide crée avec DDS est précise à 0.01 Hz (mesurée avec un fréquence mètre compteur)
 * Pour augementer la précision -> augementer les valeurs de la table, passer sur une table de 1024 ou 2048 etc...
 *
 */

#include "Dds.h"

/**
@param
 * Le Constructeur Dds
 * _slpFreq : fréquence d'échantillonage
 * _dacChannel : numero de cannal du DAC 
 * _syncLed : numero de GPIO de la syncro oscilloscope et led (led allumée = dds en fonctionnement)
*/

Dds::Dds(float _splFreq, dac_channel_t _dacChannel, gpio_num_t _syncLed) :
syncLed(_syncLed),
splFreq(_splFreq),
dacChannel(_dacChannel),
timer(NULL),
accumulateur(0),
dephase(0),
attenuation(dB_0),
incrementPhase(0),
compteur(0)        
{
    anchor = this;
}

Dds::Dds(const Dds& orig) {
}

Dds::~Dds() {
}

/**
 * @brief Dds::begin()
 *
 * @details Permet d'initialiser le dds 
 * 
 */

void Dds::begin() {

    pinMode(syncLed, OUTPUT);

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, Dds::marshall, true);
    timerAlarmWrite(timer, 1000000 / splFreq,  true);
    timerAlarmEnable(timer);

    dac_output_enable(dacChannel);
}

void Dds::marshall() {
    anchor->interuption();
}

/**
 * @brief Dds::interuption()
 *
 * @details interruption générant la sinusoide vers le dac
 * voir fiche principe du dds
 */

void IRAM_ATTR Dds::interuption() {
    const static byte sinusTable[512] PROGMEM = {128, 129, 131, 132, 134, 135, 137, 138, 140, 141, 143, 145, 146, 148, 149, 151, 152, 154, 155, 157, 158, 160, 161, 163, 164, 166, 167, 169, 170, 172, 173, 175, 176, 178, 179, 180, 182, 183, 185, 186,
        187, 189, 190, 191, 193, 194, 195, 197, 198, 199, 201, 202, 203, 204, 206, 207, 208, 209, 210, 212, 213, 214, 215, 216, 217, 218, 219, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 230, 231, 232,
        233, 234, 235, 236, 236, 237, 238, 239, 240, 240, 241, 242, 242, 243, 244, 244, 245, 245, 246, 247, 247, 248, 248, 249, 249, 249, 250, 250, 251, 251, 251, 252, 252, 252, 253, 253, 253, 253, 254, 254,
        254, 254, 254, 254, 254, 254, 254, 254, 255, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 253, 253, 253, 253, 252, 252, 252, 251, 251, 251, 250, 250, 249, 249, 249, 248, 248, 247, 247, 246, 245,
        245, 244, 244, 243, 242, 242, 241, 240, 240, 239, 238, 237, 236, 236, 235, 234, 233, 232, 231, 230, 230, 229, 228, 227, 226, 225, 224, 223, 222, 221, 219, 218, 217, 216, 215, 214, 213, 212, 210, 209,
        208, 207, 206, 204, 203, 202, 201, 199, 198, 197, 195, 194, 193, 191, 190, 189, 187, 186, 185, 183, 182, 180, 179, 178, 176, 175, 173, 172, 170, 169, 167, 166, 164, 163, 161, 160, 158, 157, 155, 154,
        152, 151, 149, 148, 146, 145, 143, 141, 140, 138, 137, 135, 134, 132, 131, 129, 127, 126, 124, 123, 121, 120, 118, 117, 115, 114, 112, 110, 109, 107, 106, 104, 103, 101, 100, 98, 97, 95, 94, 92, 91, 89,
        88, 86, 85, 83, 82, 80, 79, 77, 76, 75, 73, 72, 70, 69, 68, 66, 65, 64, 62, 61, 60, 58, 57, 56, 54, 53, 52, 51, 49, 48, 47, 46, 45, 43, 42, 41, 40, 39, 38, 37, 36, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 25, 24, 23,
        22, 21, 20, 19, 19, 18, 17, 16, 15, 15, 14, 13, 13, 12, 11, 11, 10, 10, 9, 8, 8, 7, 7, 6, 6, 6, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6,
        6, 6, 7, 7, 8, 8, 9, 10, 10, 11, 11, 12, 13, 13, 14, 15, 15, 16, 17, 18, 19, 19, 20, 21, 22, 23, 24, 25, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 36, 37, 38, 39, 40, 41, 42, 43, 45, 46, 47, 48, 49, 51, 52, 53, 54, 56, 57,
        58, 60, 61, 62, 64, 65, 66, 68, 69, 70, 72, 73, 75, 76, 77, 79, 80, 82, 83, 85, 86, 88, 89, 91, 92, 94, 95, 97, 98, 100, 101, 103, 104, 106, 107, 109, 110, 112, 114, 115, 117, 118, 120, 121, 123, 124, 126};
    uint16_t phase;
    uint8_t sinus;
    
    accumulateur += incrementPhase; // accumulateur de phase  (sur 32 bits)
    phase = ((accumulateur >> 23) + dephase) & 0x1ff; //ajoute la phase
    sinus = pgm_read_byte(&(sinusTable[phase])); //lecture de la valeur du sinus dans la table 
    sinus = (sinus >> attenuation)  - (0x80 >> attenuation) + 0x80;  //attenuation de l'amplitude la valeur moyenne reste constante
    dac_output_voltage(dacChannel, sinus); //envoi de la valeur vers le dac
    compteur++;
}

/**
 * @brief Dds::computeIncrementPhase(float freq)
 *
 * @details calcul le mot utilisé par le DDS pour générer une sinusoide à fréquence donnée
 * @param   float la frequence en hz
 */

uint32_t Dds::computeIncrementPhase(float freq) {
    return pow(2, 32) * freq / splFreq;
}

/**
 * @brief Dds::setFrequency(float freq)
 *
 * @details met a jour la fréquence de sortie du dac
 * @param   float la frequence en hz
 */

void Dds::setFrequency(float freq) {
    incrementPhase = computeIncrementPhase(freq);
}

/**
 * @brief Dds::stop()
 *
 * @details stop le dds
 */

void Dds::stop() {
    
    timerAlarmDisable(timer);
    dac_output_voltage(dacChannel, 128);
    digitalWrite(syncLed, false); 
}

void Dds::start() {
    accumulateur = 0;
    timerAlarmEnable(timer);
    digitalWrite(syncLed, true); 
}

/**
 * @brief setPhase(int ph
 *
 * @details met a jour la phase du dds
 * @param   int la phase entre 0 et 359°
 */

void Dds::setPhase(int ph) {
    dephase = round(ph * 512 / 360);
}

/**
 * @brief setAmplitude(int value)
 * @detail Valeur possible 0 1 2 3 -> (1, 1/2, 1/4, 1/8) -> (0dB, 6dB, 12dB, 18dB).
 * @param The multiple of the amplitude of the sine wave dds. The max amplitude is 3.3V.
 */
void Dds::setAttenuation(int _attenuation){
    attenuation = _attenuation;
}

/**
   @brief    Dds::off()
   @details  arrête la génération du signal
*/

void Dds::off(){
    dephase = 0;
    incrementPhase = 0;
    accumulateur = 0;
}


Dds* Dds::anchor = NULL;
