#include <LiquidCrystal.h>
#include <Arduino.h>



class ScreenInterface{
    public:
        void init(); //starts the thing
        void updateTopLine(String Text); //updates top line
        void updateBottomLine(String Text); //updates bottom line of text
        void ClearAll(); //clears screen
        void ClearTopLine(); //Clears Top Line
        void ClearBottomLine(); //Clears Bottom Lin
        void Blink();
        


    private:
        String TopLine; //stores top 16 chars of text
        String bottomLine; //stores bottom 16 lines of text
};