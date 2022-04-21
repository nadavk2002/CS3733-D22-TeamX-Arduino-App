#include <ScreenInterface.h>

//stuff to make the LCD class.
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void ScreenInterface::init(){
      lcd.begin(16,2); //initialize the LCD as 16 char long 2 tall display.
      lcd.clear();
}