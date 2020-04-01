#include <AccelStepper.h>
#include "SevenSegmentTM1637.h"
#include "SevenSegmentExtended.h"

/* initialize global TM1637 Display object
*  The constructor takes two arguments, the number of the clock pin and the digital output pin:
* SevenSegmentTM1637(byte pinCLK, byte pinDIO);
*/
const byte PIN_CLK = 11;   // define CLK pin (any digital pin)
const byte PIN_DIO = 12;   // define DIO pin (any digital pin)
SevenSegmentExtended      display(PIN_CLK, PIN_DIO);
AccelStepper motor(2,2,3);

const int buttonDetect(5);
const int buttonLED(8);

int buttonState = 0;
int flipping = 0;
byte timePerFlip = 15; //mins

// run setup code
void setup() {
  Serial.begin(9600);         // initializes the Serial connection @ 9600 baud
  display.begin();            // initializes the display
  display.setBacklight(100);  // set the brightness to 100 %
  delay(1000);                // wait 1000 ms

  pinMode(buttonDetect,INPUT);
  pinMode(buttonLED, OUTPUT);
  

  motor.setMaxSpeed(1000.0);
  motor.setAcceleration(400.0);

  //digitalWrite(buttonLED, HIGH);


};

// run loop (forever)

void loop() {

      while(flipping == 0){
        display.clear();
        display.print("OFF");

        digitalWrite(buttonLED, HIGH);
        delay(1000);
        buttonState = digitalRead(buttonDetect);
        digitalWrite(buttonLED, LOW);
        delay(1000);

        if(buttonState == 0){ //start flipping
          flipping = 1;

          digitalWrite(buttonLED, HIGH);

          display.print("LETS");
          delay(1000);
          display.print("FLIP");
          delay(1000);
          display.clear();
          delay(1000);


          break;
        }
        
      }

      byte minsLeft = timePerFlip;
      byte secsLeft = 60;

      
    
      for ( ; minsLeft > 0 ; minsLeft--){
        for(; secsLeft > 0 ; secsLeft--){

          buttonState = digitalRead(buttonDetect);
          Serial.println(buttonState);

          if(buttonState == 0){ //button pressed
            flipping = 0;
            break;
            
          }
          
          display.printTime(minsLeft-1, secsLeft-1, true);
          delay(1000); //1000 ms = 1 sec
        
       }
        secsLeft = 60;
        if(buttonState == 0){
          flipping = 0;
          break;
        }
      }

      if(flipping == 1){
        display.print("FLIP");
        motor.runToNewPosition(405*8); //.5 for perpserp
        motor.setCurrentPosition(0);
        //motor.runToNewPosition(405*8);
      }


  
};
