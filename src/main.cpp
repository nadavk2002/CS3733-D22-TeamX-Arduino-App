//fundamentally this should work that the buttons send requests when read and get updated when 
//latency will be garbage
#include <Arduino.h>

#include <Timer.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>

int status = WL_IDLE_STATUS;


const int scroll = 7;
const int enter = 8;
const int speaker = 2;


const int pitch = 500;
bool speakerOn = true;


Timer speakerTimer(500);
Timer resetScreen(3000);


void setup() {
  
  // put your setup code here, to run once:
  //start usb serial coms
  Serial.begin(115200);

  
  //start buttons
  pinMode(scroll, INPUT);
  pinMode(enter, INPUT);

  //start wifi.


}

void loop() {
  // put your main code here, to run repeatedly:
}

