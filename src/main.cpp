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



const int up = 7;
const int enter = 8;
const int down = 9;
const int speaker = 2;

ScreenInterface screen;

enum State{
  temp,
  fan,
  lamp,
  ip,
  };

  State state = ip;
  boolean editMode = false;

  String IP = "";
  String LAMP = "off";
  String FAN = "off";
  String TEMP = "72";

  bool lampState = false;
  bool fanState = false;
  double tempState = 72;
  
void setLAMP(String string);
void setFAN(String string);
void setTEMP(String string);

void handleUpPress();
void handleDownPress();
void handleEnterPress();
void updateScreenToState();

void updateRemote();

String ipToString(IPAddress ipAddress);
String boolToString(bool thing);



void setLAMP(String string){
  LAMP = string;
  if(string.equals(String("off"))){
    lampState = false;
  } else if(string.equals(String("on"))){
    lampState = true;
  }
  updateScreenToState();
  updateRemote();
}
void setFAN(String string){
  FAN = string;
  if(string.equals(String("off"))){
    fanState = false;
  } else if(string.equals(String("on"))){
    fanState = true;
  }
  updateScreenToState();
  updateRemote();
}
void setTEMP(String string){
  TEMP = string;
  tempState = string.toDouble();
  updateScreenToState();
  updateRemote();
}

void updateRemote(){
  //update TEMP
  Reply = "TEM" + String(tempState);
  Reply.toCharArray(replyBuffer, 256);
  Udp.beginPacket(WiFi.gatewayIP(), 6587);
  Udp.write(replyBuffer);
  Serial.println("PacketSent?" + Udp.endPacket());

  //update LAMP
  Reply = "LIT" + boolToString(lampState);
  Reply.toCharArray(replyBuffer, 256);
  Udp.beginPacket(WiFi.gatewayIP(), 6587);
  Udp.write(replyBuffer);
  Serial.println("PacketSent?" + Udp.endPacket());

  //update FAN
  Reply = "FAN" + boolToString(fanState);
  Reply.toCharArray(replyBuffer, 256);
  Udp.beginPacket(WiFi.gatewayIP(), 6587);
  Udp.write(replyBuffer);
  Serial.println("PacketSent?" + Udp.endPacket());


}

void handleUpPress(){
  switch (state){
    case temp:
    state = ip;
    break;
    case fan:
    state = temp;
    break;
    case lamp:
    state = fan;
    break;
    case ip:
    state = lamp;
    break;
    default:
    state = ip;
    break;
  }
  updateScreenToState();
}

void handleDownPress(){
  switch (state){
    case temp:
    state = fan;
    break;
    case fan:
    state = lamp;
    break;
    case lamp:
    state = ip;
    break;
    case ip:
    state = temp;
    break;
    default:
    state = ip;
    break;
  }
  updateScreenToState();
}
void handleEnterPress(){

}

void updateScreenToState(){
  switch (state){
    case temp:
    screen.updateTopLine("Temperature:");
    screen.updateBottomLine(TEMP);
    break;
    case fan:
    screen.updateTopLine("Fan:");
    screen.updateBottomLine(boolToString(fanState));
    break;
    case lamp:
    screen.updateTopLine("Lamp");
    screen.updateBottomLine(boolToString(lampState));
    break;
    case ip:
    screen.updateTopLine("ip Adress");
    screen.updateBottomLine(IP);
    break;
    default:
    screen.updateTopLine("ip Adress");
    screen.updateBottomLine(IP);
    break;
  }
  Serial.println(fanState);
}

String ipToString(IPAddress ipAddress){
  return String(ipAddress[0]) + String(".") +
           String(ipAddress[1]) + String(".") +
           String(ipAddress[2]) + String(".") +
           String(ipAddress[3]);
}

String boolToString(bool thing){
  if(thing){
    return String("on");
  }
  return String("off");
}




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
  pinMode(up, INPUT);
  pinMode(enter, INPUT);
  pinMode(down, INPUT);

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

  IP = ipToString(WiFi.localIP());


  Udp.begin(6587);

  delay(250);
  updateScreenToState();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //if button is pressed send udp packet
  // if(digitalRead(enter) == HIGH){
  //   tone(speaker, 500);
  //   Reply = "hello";
  //   Reply.toCharArray(replyBuffer, 256);
  //   Udp.beginPacket(WiFi.gatewayIP(), 6587);
  //   Udp.write(replyBuffer);
  //   Serial.println(Udp.endPacket());
  //   while(digitalRead(enter) == HIGH){
  //     ;
  //   }
  //   noTone(speaker);
  
  // } else if(digitalRead(up) == HIGH){
  //   tone(speaker, 600);
  //   Reply = "getData";
  //   Reply.toCharArray(replyBuffer, 256);
  //   Udp.beginPacket(WiFi.gatewayIP(), 6587);
  //   Udp.write(replyBuffer);
  //   Serial.println(Udp.endPacket());
  //   while(digitalRead(up) == HIGH){
  //     ;
  //   }
  //   noTone(speaker);
  // }else if(digitalRead(down) == HIGH){
  //   tone(speaker, 600);
  //   Reply = "getLIGHT";
  //   Reply.toCharArray(replyBuffer, 256);
  //   Udp.beginPacket(WiFi.gatewayIP(), 6587);
  //   Udp.write(replyBuffer);
  //   Serial.println(Udp.endPacket());
  //   while(digitalRead(down) == HIGH){
  //     ;
  //   }
  //   noTone(speaker);
  // }

  //handle button presses
  if(digitalRead(up) == HIGH){
    handleUpPress();
    while(digitalRead(up) == HIGH){;}
  }
  if(digitalRead(down) == HIGH){
    handleDownPress();
    while(digitalRead(down) == HIGH){;}
  }
  if(digitalRead(enter) == HIGH){
    handleEnterPress();
    while(digitalRead(enter) == HIGH){;}
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
    Serial.print("Contents: ");
    Serial.println(packetBuffer);

    // screen.clearAll();
    // screen.updateBottomLine(packetBuffer);

    //determine where the packet should go
    //get type
    char typeText[3] = "";
    for(int i = 0; i <= 2; i++){
      typeText[i] = packetBuffer[i];
    }
    //get info
    char infoText[253] = "";
    for(int i = 3; i <= sizeof(packetBuffer); i++){
      infoText[i-3] = packetBuffer[i];
    }

    //screen.updateBottomLine(infoText);
    Serial.println(infoText);

    String type = String(typeText);
    String compare = String("TEM");
    //sort it out
    if(type.equals(compare)){
      setTEMP(infoText);
    }
    compare = String("LIT");
    if(type.equals(compare)){
      setLAMP(infoText);
    }
    compare = String("FAN");
    if(type.equals(compare)){
      setFAN(infoText);
    }
    compare = String("GET");
    if(type.equals(compare)){
      updateRemote();
    }

    //screen.updateBottomLine(infoText);

    

  }


  delay(100);
}

