/* Encoder Library
 * This example code is in the public domain.
 */

#include <Encoder.h> 
// Uses 5um (0.005mm) incremental glass scale linear encoder

// Spread interupt pins 2 & 3 accross each scale, 
Encoder scale1(2, 4); 
Encoder scale2(3, 5);

const int ZincPin = 6; 
const int ZeroPin =7; 

int ZeroPinState = 0;
int ZincPinState = 0;

long ref1 = 0; // reference value
long ref2 = 0; // reference value
long ref3 = 0; // reference value
long incpos = 0;
//long abs = 0;
long inc = 0;
long Zinc = 0;
long Zero = 0;
long oldPosition1  = 0;
long oldPosition2  = 0;
long oldPosition3 = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Zero set button:");
  pinMode(ZeroPin, INPUT_PULLUP);
  pinMode(ZincPin, INPUT_PULLUP);
  
}

void loop() {
  ZeroPinState = digitalRead(ZeroPin);
     if (ZeroPinState == CHANGE) {
      zero();
         }
     else {
  long newPosition1 = scale1.read();
     if (newPosition1 != oldPosition1) 
         oldPosition1 = newPosition1;
    
long newPosition2 = scale2.read();
  if (newPosition2 != oldPosition2) 
    oldPosition2 = newPosition2;

     ref1 = newPosition1;
     ref2 = newPosition2;
 
    Serial.print(ref1/200.00,3),Serial.print("mm X-Slide : "),
    Serial.print(scale1.read()/200.00,3),Serial.print("  scale1  :");
    Serial.print(ZeroPinState), Serial.println("--ZeroPinstate");
    //Serial.print(Zref2/200.00,3),Serial.println("mm Y-Slide");
      
  }
}
void zero() {

    ZincPinState = digitalRead(ZincPin);
     if (ZincPinState == CHANGE) {
      long newPosition3 = scale1.read();
        (newPosition3 != oldPosition3) ;
         oldPosition3 = newPosition3;
         Zinc = (newPosition3-ref1);
          Serial.print(Zinc/200.00,3),Serial.print("  =Zinc=  ");
          Serial.print(ref1/200.00,3),Serial.print("  =ref1= : ");
          Serial.print(newPosition3/200.00,3),Serial.print("  ==newpos3==  ");
          Serial.print(scale1.read()/200.00,3), Serial.print("  ==scale1==  ");
          Serial.print(ZincPinState), Serial.println("=ZincPinstate ~~~ void zero() ");
          
                }

     else {  //--PinState 0
     //scale1.write(0);
    long newPosition3 = scale1.read();
     if (newPosition3 != oldPosition3) 
     //if (newPosition3 != ref1) 
         oldPosition3 = newPosition3;
         incpos = (scale1.read() - ref1);
         Zero = (newPosition3-incpos);
         Serial.print(incpos/200.00,3),Serial.print("  incpos :    ");
         Serial.print(Zero/200.00,3),Serial.print("  Zero  ");

         Serial.print(newPosition3/200.00,3),Serial.print("  newpos3 :    ");
         Serial.print(ref1/200.00,3), Serial.print("  ref1  ");
         Serial.print(scale1.read()/200.00,3),Serial.print("  scale1  ");
         Serial.print(ZincPinState), Serial.println("=ZincPinstate ~+~ void zero() ");
     //delay(1000)
     }
     }
