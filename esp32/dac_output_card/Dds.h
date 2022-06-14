/* 
 * File:   Dds.h
 * Author: Anthony (f4goh@orange.fr) & philippe S (philaure@wanadoo.fr)
 *
 * Created on 23 juillet 2021, 11:35
 */

#ifndef DDS_H
#define DDS_H
#include <Arduino.h>
#include <driver/dac.h>

#define SAMPLING_FREQUENCY 25000    //fréquence d'échantillonage
#define DAC_CHANNEL DAC_CHANNEL_1   //numéro du dac
#define SYNC GPIO_NUM_2             //gpio pour led

//#if ((10000000/SAMPLING_FREQUENCY)%10>0)
//#error "Fréquence d'échantillonage incompatible avec la période du timer"
//#endif

typedef enum {
    dB_0  = 0x0,   /* 1/1 Default value */
    dB_6  = 0x1,   /* 1/2 */
    dB_12 = 0x2,   /* 1/4 */
    dB_18 = 0x3,   /* 1/8 */
    dB_24 = 0x4,   /* 1/16 */        
} attenuation_t;

class Dds {
public:
    Dds(float _splFreq = SAMPLING_FREQUENCY,  
        dac_channel_t _dacChannel = DAC_CHANNEL_1, 
        gpio_num_t _syncLed = GPIO_NUM_2);
    
    Dds(const Dds& orig);
    virtual ~Dds();

    void begin();
    
    void setFrequency(float freq);
    void stop();
    void start();
    void off();
    void setPhase(int ph);
    void setAttenuation(int _attenuation);


private:
    hw_timer_t * timer;
    void IRAM_ATTR interuption();
    static void marshall();
    

    static Dds* anchor;
    gpio_num_t syncLed;
    dac_channel_t dacChannel;
    volatile uint32_t accumulateur;        // Accumulateur de phase
    
protected:
    uint32_t computeIncrementPhase(float freq);
    
    volatile uint32_t incrementPhase;      // Increment de phase courant
    volatile uint32_t dephase;             // Valeur du déphasage de la porteuse  
    volatile uint32_t compteur;            // Compteur d'échantillons
    int splFreq;                           // Fréquence d'échantillonage
    int attenuation;                       // Attenuation de l'amplitude (1, 1/2, 1/4, 1/8) soit (0dB, 6dB, 12dB, 18 dB)                 
};

#endif /* DDS_H */