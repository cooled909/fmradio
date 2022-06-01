#include "screen.h"
void drawStereo(){

  char newTimeString[MaxString] = { 0 };

  dtostrf(freq, 3, 1, newTimeString);
  
  if (strcmp(newTimeString,oldTimeString) != 0) {

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
    }
}
