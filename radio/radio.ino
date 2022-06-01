#define uint8_t byte
#include <TEA5767.h>
#include <Wire.h>
#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <EEPROM.h>
#include <DS3231.h>

  #define TFT_CS        10
  #define TFT_RST        9 // Or set to -1 and connect to Arduino RESET pin
  #define TFT_DC         8


#define SerialDebugging true

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

const uint16_t  Display_Color_Black        = 0x0000;
const uint16_t  Display_Color_Blue         = 0x001F;
const uint16_t  Display_Color_Red          = 0xF800;
const uint16_t  Display_Color_Green        = 0x07E0;
const uint16_t  Display_Color_Cyan         = 0x07FF;
const uint16_t  Display_Color_Magenta      = 0xF81F;
const uint16_t  Display_Color_Yellow       = 0xFFE0;
const uint16_t  Display_Color_White        = 0xFFFF;

uint16_t        Display_Text_Color         = Display_Color_Green;
uint16_t        Display_Background_Color    = Display_Color_Black;

bool            isDisplayVisible           = false;

const size_t MaxString = 16;

char oldSignalString[MaxString] = { 0 };

char oldTimeString[MaxString] = { 0 };

TEA5767 radio = TEA5767();
int buttonA = 3;
int buttonB = 2;
int buttonC = 5;
int buttonD = 6;
int buttonE = 4;

float freq = 96.7;

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 50; 
int count = 0;
int buttonPressed;

float fav1;
float fav2;
float fav3;

int prevMonth;
int prevDay;
int prevHour;
int prevMinute;

DS3231 clock;
RTCDateTime dt;

void savedStationDraw(){
  tft.setTextSize(3);
  tft.setTextColor(Display_Background_Color);
  tft.setCursor(0,80);
  tft.print(prevMonth);
  tft.print("/");
  tft.print(prevDay);
  tft.setCursor(0,105);
  if(prevHour > 12){
    tft.print(prevHour-12);
  }
  else{
    tft.print(prevHour);
  }
  tft.print(":");
  if(prevMinute < 10){
    tft.print("0");
    tft.print(prevMinute);
  }
  else{
    tft.print(prevMinute);  
  }
  if(prevHour > 12){
    tft.print("PM");
  }
  else{
    tft.print("AM");
  }

  tft.setCursor(0,90);
  tft.setTextSize(2);
  tft.setTextColor(Display_Text_Color);
  tft.print("Station \nsaved");
  delay(2000);
  tft.setCursor(0,90);
  tft.setTextColor(Display_Background_Color);
  tft.print("Station \nsaved");
  
  tft.setTextSize(3);
  tft.setTextColor(Display_Text_Color);
  tft.setCursor(0,80);
  tft.print(dt.month);
  tft.print("/");
  tft.print(dt.day);
  tft.setCursor(0,105);
  if(dt.hour > 12){
    tft.print(dt.hour-12);
  }
  else{
    tft.print(dt.hour);
  }
  tft.print(":");
  if(dt.minute < 10){
    tft.print("0");
    tft.print(dt.minute);
  }
  else{
    tft.print(dt.minute);  
  }
  if(dt.hour > 12){
    tft.print("PM");
  }
  else{
    tft.print("AM");
  }
}
  

void clockDraw(){
  dt = clock.getDateTime();

  if(dt.minute != prevMinute){
  tft.setTextSize(3);
  tft.setTextColor(Display_Background_Color);
  tft.setCursor(0,80);
  tft.print(prevMonth);
  tft.print("/");
  tft.print(prevDay);
  tft.setCursor(0,105);
  if(prevHour > 12){
    tft.print(prevHour-12);
  }
  else{
    tft.print(prevHour);
  }
  tft.print(":");
  if(prevMinute < 10){
    tft.print("0");
    tft.print(prevMinute);
  }
  else{
    tft.print(prevMinute);  
  }
  if(prevHour > 11){
    tft.print("PM");
  }
  else{
    tft.print("AM");
  }
  
  tft.setTextSize(3);
  tft.setTextColor(Display_Text_Color);
  tft.setCursor(0,80);
  tft.print(dt.month);
  tft.print("/");
  tft.print(dt.day);
  tft.setCursor(0,105);
  if(dt.hour > 12){
    tft.print(dt.hour-12);
  }
  else{
    tft.print(dt.hour);
  }
  tft.print(":");
  if(dt.minute < 10){
    tft.print("0");
    tft.print(dt.minute);
  }
  else{
    tft.print(dt.minute);  
  }
  if(dt.hour > 11){
    tft.print("PM");
  }
  else{
    tft.print("AM");
  }

  prevMonth = dt.month;
  prevDay = dt.day;
  prevHour = dt.hour;
  prevMinute = dt.minute;
  }
}

void drawStereo(float freq){

  char newTimeString[MaxString] = { 0 };

  dtostrf(freq, 3, 1, newTimeString);

  
  if (strcmp(newTimeString,oldTimeString) != 0) {

      char newSignalString[MaxString] = { 0 };

      sprintf(
        newSignalString,
        "Signal: %d",
        radio.getSignalLevel()
        );

        tft.setTextSize(2);
        tft.setCursor(0,25);

        tft.setTextColor(Display_Background_Color);

        tft.print(oldSignalString);

        tft.setCursor(0,25);

        tft.setTextColor(Display_Text_Color);

        tft.print(newSignalString);

        tft.setTextSize(4);
    
        // yes! home the cursor
        tft.setCursor(0,50);

        // change the text color to the background color
        tft.setTextColor(Display_Background_Color);

        // redraw the old value to erase
        tft.print(oldTimeString);

        // home the cursor
        tft.setCursor(0,50);
        
        // change the text color to foreground color
        tft.setTextColor(Display_Text_Color);
    
        // draw the new time value
        tft.print(newTimeString);
        Serial.println(newTimeString);
        // and remember the new value
        strcpy(oldTimeString,newTimeString);
        strcpy(oldSignalString,newSignalString);
    }
}

void setup()
{ 
  delay(250);

  tft.initR(INITR_144GREENTAB);

  tft.setFont();
  tft.fillScreen(Display_Background_Color);
  tft.setTextColor(Display_Text_Color);
  tft.setTextSize(4);
  tft.setRotation(2);
  
  Wire.begin();
  radio.setFrequency(freq); // pick your own frequency
  drawStereo(freq);

  clock.begin();

  //clock.setDateTime(2022, 5, 30, 11, 9, 30);
  //for testing
  
  //clock.setDateTime(__DATE__, __TIME__);
  //for setting time
  
  clockDraw();

  pinMode(buttonA, INPUT_PULLUP);
  pinMode(buttonB, INPUT_PULLUP);
  pinMode(buttonC, INPUT_PULLUP);
  pinMode(buttonD, INPUT_PULLUP);
  pinMode(buttonE, INPUT_PULLUP);


  startMillis = millis();
}

enum ST_States {ST_Start, ST_Wait, ST_Next, ST_Prev, ST_Hold, ST_Fav, ST_SwitchToFav, ST_NewFav} ST_State;

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
      else if(digitalRead(buttonC) == LOW || digitalRead(buttonD) == LOW || digitalRead(buttonE) == LOW){
        ST_State = ST_Fav;
      }
      else{
        ST_State = ST_Wait;
      }
     break;
     case ST_Next:
      if(radio.getSignalLevel() < 7){
        ST_State = ST_Next;
      }
      else if(radio.getSignalLevel() >= 7){
      ST_State = ST_Hold;
     }
     break;
     case ST_Prev:
      if(radio.getSignalLevel() < 7){
        ST_State = ST_Prev;
      }
      else if(radio.getSignalLevel() >= 7){
      ST_State = ST_Hold;
      }
     break;
     case ST_Hold:
      if(digitalRead(buttonB) == HIGH && digitalRead(buttonA) == HIGH &&digitalRead(buttonC) == HIGH && digitalRead(buttonD) == HIGH && digitalRead(buttonE) == HIGH){
        ST_State = ST_Wait;
      }
      else{
        ST_State = ST_Hold;
      }
      break;
    case ST_Fav:
      if(count == 40){
        ST_State = ST_NewFav;
        count = 0;
      }
      else if(digitalRead(buttonC) == LOW || digitalRead(buttonD) == LOW || digitalRead(buttonE) == LOW){
        ST_State = ST_Fav;
        count++;
      }
      else if(digitalRead(buttonC) == HIGH && digitalRead(buttonD) == HIGH && digitalRead(buttonE) == HIGH){
        ST_State = ST_SwitchToFav;
        count = 0;
      }
      break;
      case ST_SwitchToFav:
        ST_State = ST_Wait;
      break;
      case ST_NewFav:
        ST_State = ST_Hold;  
      break;
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
    case ST_Fav:
      if(digitalRead(buttonC) == LOW){
        buttonPressed = 1;
      }
      else if(digitalRead(buttonD) == LOW){
        buttonPressed = 2;
      }
      else if(digitalRead(buttonE) == LOW){
        buttonPressed = 3;
      }
    break;
    case ST_SwitchToFav:
      if(buttonPressed == 1){
        freq = fav1;
        radio.setFrequency(freq);
      }
      else if(buttonPressed == 2){
        freq = fav2;
        radio.setFrequency(freq);
      }
      else if(buttonPressed == 3){
        freq = fav3;
        radio.setFrequency(freq);
      }
    break;
    case ST_NewFav:
      if(digitalRead(buttonC) == LOW){
        fav1 = freq;
      }
      else if(digitalRead(buttonD) == LOW){
        fav2 = freq;
      }
      else if(digitalRead(buttonE) == LOW){
        fav3 = freq;
      }
      savedStationDraw();
    break;
  }
}

void loop()
{
  currentMillis = millis();
  if(currentMillis - startMillis >=period){
    TickFct_Stereo();
    drawStereo(freq);
    clockDraw();
    startMillis = currentMillis;
  }
}
