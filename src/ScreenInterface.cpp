#include <ScreenInterface.h>

//stuff to make the LCD class.
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void ScreenInterface::init(){
      lcd.begin(16,2); //initialize the LCD as 16 char long 2 tall display.
      lcd.clear();
}
void ScreenInterface::clearAll(){
      lcd.clear();
      topLine = "";
      bottomLine = "";
}

void ScreenInterface::clearTopLine(){
      lcd.clear();
      topLine = "";
      lcd.setCursor(0,1);
      lcd.print(bottomLine);
}

void ScreenInterface::clearBottomLine(){
      lcd.clear();
      bottomLine = "";
      lcd.setCursor(0,0);
      lcd.print(topLine);
}

void ScreenInterface::updateTopLine(String text){
      clearTopLine();
      lcd.setCursor(0,0);
      lcd.print(text);
      topLine = text;
}

void ScreenInterface::updateBottomLine(String text){
      clearBottomLine();
      lcd.setCursor(0,1);
      lcd.print(text);
      bottomLine = text;
}