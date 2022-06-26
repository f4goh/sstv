/********************************************************************************************
 * Arduino library for AD9850
 * Created 23/08/2014
 * Christophe Caiveau f4goj@free.fr 
 * Upgrade to SPI Anthony F4GOH
 *
 * This library uses the Serial Peripheral Interface (SPI) to accelerate the update
 * of the AD9850 18.8µs (3.6µs for the deltaphase calculation)
 *
 * Use this library freely
 *
 * Hardware connections : 
 * W_CLK   -> D13 arduino UNO/NANO, D52 MEGA
 * FQ_UD   -> any pin except 10 and 12 UNO/NANO, 50 and 53 MEGA
 * DATA/D7 -> D11 arduino UNO/NANO, D51 MEGA
 * RESET   -> any pin except 10 and 12 UNO/NANO, 50 and 53 MEGA
 *
 * Functions :
 * DDS.begin(); Initialize the output pins and master reset the AD9850 
 * DDS.calibrate(frequency); Compensation of crystal oscillator frequency
 * DDS.setfreq(frequency,phase); frequency in Hz, phase coded on 5 bits
 * DDS.down(); power down mode reducing the dissipated power from 380mW to 30mW @5V
 * DDS.up(); wake-up the AD9850
 *
 * AD9850 datasheet at http://www.analog.com/static/imported-files/data_sheets/AD9850.pdf
 *
 *******************************************************************************************/

#include <AD9850SPI.h>
#include <SPI.h>


AD9850SPI::AD9850SPI():
deltaphase(0),
phase(0),
calibFreq(125000000)        
{
	
}

/**
   @brief  AD9850SPI::begin()
   @details initialise le dds et le bus spi à 16Mhz   
 */

void AD9850SPI::begin() {
        SPI.begin();        
	pinMode(W_CLK_PIN, OUTPUT);
	pinMode(FQ_UD_PIN, OUTPUT);
	pinMode(RESET_PIN, OUTPUT);
	
        //SPI.setClockDivider(SPI_CLOCK_DIV2);  //8mhz
        //ou
        SPI.setFrequency(16000000);
	SPI.setBitOrder(LSBFIRST);
	SPI.setDataMode(SPI_MODE0);

	pulse(RESET_PIN);
	pulse(W_CLK_PIN);
	pulse(FQ_UD_PIN);
        setfreq(0,0);
        setfreq(0,0);
}

/**
   @brief  AD9850SPI::update()
   @details met a jour le dds word deltaphase dans le dds   
 */

void AD9850SPI::update() {
        SPI.transfer32(deltaphase);
	SPI.transfer(phase & 0xFF);
	pulse(FQ_UD_PIN);
}

/**
   @brief  AD9850SPI::setfreq(double f, uint8_t p)
   @details met a jour al fréquence du dds
   @param   double f : fréquence
            uint8_t p : phase
 */

void AD9850SPI::setfreq(double f, uint8_t p) {
	deltaphase = f * 4294967296.0 / calibFreq;
	phase = p << 3;
	update();
}

/**
   @brief  AD9850SPI::update()
   @details met en sommeil le dds
 */


void AD9850SPI::down() {
	pulse(FQ_UD_PIN);
	SPI.transfer(0x04);
	pulse(FQ_UD_PIN);        
}
    

void AD9850SPI::up() {
	update();
}


void AD9850SPI::calibrate(double TrimFreq)
{
	calibFreq = TrimFreq;
}

void AD9850SPI::powerOn(){
  //pinMode(MOSI_PIN, OUTPUT); //a vérifier
  delay(100);  
  wakeUp();
  delay(100);
  update();
}

void AD9850SPI::powerOff(){
 down();   
 //pinMode(MOSI_PIN, INPUT); //a vérifier
}

void AD9850SPI::wakeUp()
{
 pulse(RESET_PIN);
 pulse(W_CLK_PIN);
 pulse(FQ_UD_PIN); 
}

void AD9850SPI::pulse(int pin) {
	digitalWrite(pin, HIGH);
	digitalWrite(pin, LOW);
}

void AD9850SPI::vna(unsigned long DS_FTW,uint8_t Mode) {
 deltaphase=(double) DS_FTW;
 phase=Mode;
 update();
}
