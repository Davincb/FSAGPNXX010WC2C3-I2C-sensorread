#include "Wire.h"
#include <Arduino.h>
#include <Wire.h>

#define FSAGPNXX010WC2C3_I2C 0x28 // each I2C object has a unique bus address, the DS1307 is 0x68
#define OUTPUT_MIN 3276       //  (20% of 2^14 counts or 0x0666)
#define OUTPUT_MAX 13107       // (80% of 2^14 counts or 0x3999)
#define FORCE_MIN 0.32        // min is 0 for sensors that give absolute values
#define FORCE_MAX 10   // 10N (I want results in N)




void setup()
{
 Wire.begin(); // wake up I2C bus
 delay (500);
 Serial.begin(9600);
}



void loop()
{
 float force;
 //send a request
Wire.beginTransmission(FSAGPNXX010WC2C3_I2C); 
Wire.write(1);  // send a bit asking for register one, the data register (as specified by the pdf)
Wire.endTransmission(); v
delay (20);

Wire.requestFrom(FSAGPNXX010WC2C3_I2C, 4);
while(Wire.available() == 0);
byte a     = Wire.read(); // first received byte stored here ....Example bytes one: 00011001 10000000
byte b     = Wire.read(); // second received byte stored here ....Example bytes two: 11100111 00000000
byte c     = Wire.read(); // third received byte stored here
byte d     = Wire.read(); // fourth received byte stored here

// Serial.println(a, BIN);
 //Serial.println(b, BIN);
  // Serial.println(c, BIN);
   // Serial.println(d, BIN);

 byte status1 = (a & 0xc0) >> 6; 
//Serial.println(status1, BIN);

int bridge_data = ((a & 0x3f) << 8) + b;
           
           if ( status1 == 1 ) {
               Serial.println("warn command mode ");// *Command mode is used for programming the sensor. This mode should not be seen during normal operation.
              // Serial.println(status, BIN);  
           }
           
           if ( status1 == 2 ) {    
               Serial.println("warn stale data ");  // if data has already been feched since the last measurement cycle
              // Serial.println(status, BIN);
           }  
           
           if ( status1 == 3) {
               Serial.println("err diagnostic fault "); //When the two status bits are "11", one of the above mentioned diagnostic faults is indicated.
              // Serial.println(status, BIN);
           }


   force = 1.0 * (bridge_data - OUTPUT_MIN) * (FORCE_MAX - FORCE_MIN) / (OUTPUT_MAX - OUTPUT_MIN) + FORCE_MIN;
   

   
   
           Serial.print("status      ");
           Serial.println(status1, BIN);
           Serial.print("bridge_data ");
           Serial.println(bridge_data, DEC);
        

           Serial.print("Force    (N) ");
           Serial.println(force);
           
           
           
           delay (500);
}
