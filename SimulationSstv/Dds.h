/* 
 * File:   Dds.h
 * Author: Anthony (f4goh@orange.fr) & philippe S (philaure@wanadoo.fr)
 *
 * Created on 23 juillet 2021, 11:35
 */

#ifndef DDS_H
#define DDS_H

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iostream>
#include <math.h>

#define DDS_SAMPLING_FREQUENCY 1000000.0    //fréquence d'échantillonage

typedef enum {
    dB_0  = 0x0,   /* 1/1 Default value */
    dB_6  = 0x1,   /* 1/2 */
    dB_12 = 0x2,   /* 1/4 */
    dB_18 = 0x3,   /* 1/8 */
    dB_24 = 0x4,   /* 1/16 */        
} attenuation_t;

class Dds {
public:
    Dds(float _splFreq = DDS_SAMPLING_FREQUENCY);
    
    Dds(const Dds& orig);
    virtual ~Dds();


    
    void setFrequency(float freq);
    //void stop();
    //void start();
    void off();
    void setPhase(int ph);
    void setAttenuation(int _attenuation);
    uint8_t simulationInteruption();
    
    void raz();
    
private:
    

    uint32_t accumulateur;        // Accumulateur de phase
    
protected:
    uint32_t computeIncrementPhase(float freq);
    uint32_t compteur;            // Compteur d'échantillons
    uint32_t incrementPhase;      // Increment de phase courant
    uint32_t dephase;             // Valeur du déphasage de la porteuse  
    
    int splFreq;                           // Fréquence d'échantillonage
    int attenuation;                       // Attenuation de l'amplitude (1, 1/2, 1/4, 1/8) soit (0dB, 6dB, 12dB, 18 dB)                 
};

#endif /* DDS_H */