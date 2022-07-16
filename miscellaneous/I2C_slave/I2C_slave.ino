/*
 I2C Slave mode test success
 Arduino pro or pro mini
*/


#include <Wire.h>

#define I2C_SLAVE 0x70 //must be after 0x30 for esp32cam

//structure pour transférer les coordonnées
typedef struct
{
  float latitude;
  float longitude;
  uint32_t altitude;
} coordonnees;

coordonnees coord;


void requestEvent() { 
 int i; 
 for(i = 0; i < sizeof(coordonnees); i++){
   Wire.write(*((char*)&coord + i));
   //Serial.print(*((char*)&coord + i),HEX);
 } 
}


void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  Wire.begin(I2C_SLAVE);                // join i2c bus with address 0x70
  Wire.onRequest(requestEvent);
  coord.latitude=47.890242;
  coord.longitude=0.276770;
  coord.altitude=15230;
  Serial.print(sizeof(coordonnees));
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
