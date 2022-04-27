//fundamentally this should work that the buttons send requests when read and get updated when 
//latency will be garbage
#include <Arduino.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>

#include <Timer.h>
#include <WiFi_config.h>
#include <ScreenInterface.h>


int status = WL_IDLE_STATUS;
//do not actually put the SSID or Password here!
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;    

WiFiUDP Udp;
//IPAddress server(192,168,137,1);

String Reply;       // a string to send back
char replyBuffer[256];
char packetBuffer[256]; //buffer to hold incoming packet



const int scroll = 7;
const int enter = 8;
const int speaker = 2;

ScreenInterface screen;




void setup() {
  //start screen
  screen.init();

  
  // put your setup code here, to run once:
  //start usb serial coms
  Serial.begin(115200);

  // //for debugging, remove this at some point after verifying system works while plugged into computer
  //   while (!Serial) {
  //   ; // wait for serial port to connect. Needed for native USB port only
  // }

  
  //start buttons
  pinMode(scroll, INPUT);
  pinMode(enter, INPUT);

  //start wifi.
  //the network being connected to is a 2.4ghz being created by a windows 10 device using the mobile hotspot feature
    // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    screen.updateTopLine("WiFi Module");
    screen.updateBottomLine("not found");
    // don't continue
    while (true);
  }
  
    String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
    screen.updateTopLine("upgradeFW");
  }

  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);


  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    screen.updateTopLine("connecting to:");
    screen.updateBottomLine(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(500);
  }

  Serial.println("connected to network");
  screen.updateTopLine("connected!");

  Serial.print("ip: ");
  Serial.println(WiFi.localIP());

  Serial.print("gateway: ");
  Serial.println(WiFi.gatewayIP()); //the gateway will always be the java udp server due to the way this network is configured.

  Udp.begin(6587);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //if button is pressed send udp packet
  if(digitalRead(enter) == HIGH){
    tone(speaker, 500);
    Reply = "hello";
    Reply.toCharArray(replyBuffer, 256);
    Udp.beginPacket(WiFi.gatewayIP(), 6587);
    Udp.write(replyBuffer);
    Serial.println(Udp.endPacket());
    while(digitalRead(enter) == HIGH){
      ;
    }
    noTone(speaker);
  
  } else if(digitalRead(scroll) == HIGH){
    tone(speaker, 600);
    Reply = "getData";
    Reply.toCharArray(replyBuffer, 256);
    Udp.beginPacket(WiFi.gatewayIP(), 6587);
    Udp.write(replyBuffer);
    Serial.println(Udp.endPacket());
    while(digitalRead(scroll) == HIGH){
      ;
    }
    noTone(speaker);
  }

  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    screen.clearAll();
    screen.updateTopLine(packetBuffer);
  }


  delay(100);
}

