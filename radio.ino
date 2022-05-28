// TEA5767 Example

#include <Wire.h>
#include <TEA5767Radio.h>
#include "Arduino.h"


TEA5767Radio radio = TEA5767Radio();
int buttonA = 3;
int buttonB = 2;
float freq = 96.7;

void setup()
{ 
  Wire.begin();
  radio.setFrequency(freq); // pick your own frequency

  pinMode(buttonA, INPUT_PULLUP);
  pinMode(buttonB, INPUT_PULLUP);
  
}

enum ST_States {ST_Start, ST_Wait, ST_Next, ST_Prev, ST_Hold} ST_State;

void TickFct_Stereo(){
  switch(ST_State){ //transitions
    case ST_Start:
      ST_State = ST_Wait;
     break;
    case ST_Wait:
      if(digitalRead(buttonA)==LOW){
        ST_State = ST_Prev;
      }
      else if(digitalRead(buttonB)==LOW){
        ST_State = ST_Next;
      }
      else{
        ST_State = ST_Wait;
      }
     break;
     case ST_Next:
      ST_State = ST_Hold;
     break;
     case ST_Prev:
      ST_State = ST_Hold;
     break;
     case ST_Hold:
      if(digitalRead(buttonB) == HIGH && digitalRead(buttonA) == HIGH){
        ST_State = ST_Wait;
      }
      else{
        ST_State = ST_Hold;
      }
      default:
        ST_State = ST_Start;
      break;
  }
  switch(ST_State){ //actions
    case ST_Next:
      freq += .1;
      radio.setFrequency(freq);
    break;
    case ST_Prev:
      freq -= .1;
      radio.setFrequency(freq);
    break;
  }
}

void loop()
{
  TickFct_Stereo();
}
