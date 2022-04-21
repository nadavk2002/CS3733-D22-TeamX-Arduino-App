#include <Timer.h>

//constructor
Timer::Timer(unsigned long int length){
    unsigned long int currTime = millis();
    resetTime = currTime + length;
    duration = length;
}
//check timer
bool Timer::isExpired(){
    unsigned long int currTime = millis();
    if(currTime >= resetTime){
        resetTime = currTime + duration;
        return true;
    }
    return false;
}

//reset
void Timer::reset(){
    resetTime = millis() + duration;
}