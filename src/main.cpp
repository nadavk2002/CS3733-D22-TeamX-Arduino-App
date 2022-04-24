//fundamentally this should work that the buttons send requests when read and get updated when 
//latency will be garbage
#include <Arduino.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>

#include <Timer.h>
#include <WiFi_config.h>


int status = WL_IDLE_STATUS;
//do not actually put the SSID or Password here!
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;    



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

  //for debugging, remove this at some point after verifying system works while plugged into computer
    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  
  //start buttons
  pinMode(scroll, INPUT);
  pinMode(enter, INPUT);

  //start wifi.
  //the network being connected to is a 2.4ghz being created by a windows 10 device using the mobile hotspot feature

    // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }
  
    String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(500);
  }

  Serial.println("connected to network");



}

void loop() {
  // put your main code here, to run repeatedly:
}

