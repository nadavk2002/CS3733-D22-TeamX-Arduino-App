#include <Arduino.h>

class Timer{
    public:
        Timer(unsigned long int length); //constuctor
        bool isExpired(); //checks to see if timer has expired
        void reset(); //resets timer
    private:
     long unsigned int resetTime;
     long unsigned int duration; //duration in milliseconds
};
