#include <MicroGear.h>
#include <ESP8266WiFi.h>
#include "Wire.h"
#include "SH1106.h"

//const char* ssid     = "BS-IOT";
const char* ssid     = "GCS Phone";
const char* password = "1234567890";

//netpie.io
#define APPID   "APPID"
#define KEY     "APPKEY"
#define SECRET  "SECRET"

#define ALIAS   "NodeMCU1"
#define TargetWeb "HTML_web"
#define Topic "/bs/msg"


WiFiClient client;
MicroGear microgear(client);
SH1106 display(0x3c, D6, D7);

int disp=0;

void beep(int lev)
{
  if(lev==1){
    digitalWrite(D5, HIGH);
    delay(100);
    digitalWrite(D5, LOW);
    delay(100);
    digitalWrite(D5, HIGH);
    delay(100);
    digitalWrite(D5, LOW);
  }else if(lev==2){
    digitalWrite(D5, HIGH);
    delay(100);
    digitalWrite(D5, LOW);
    delay(100);
    digitalWrite(D5, HIGH);
    delay(100);
    digitalWrite(D5, LOW);
    delay(100);
    digitalWrite(D5, HIGH);
    delay(500);
    digitalWrite(D5, LOW);
  }else{
    digitalWrite(D5, HIGH);
    delay(100);
    digitalWrite(D5, LOW);
  }
}

void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) 
{
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0';
    Serial.println((char *)msg);
    String txt = String((char *)msg);

    if(txt == "!"){
      beep(0);
    }else if(txt == "!1"){
      beep(1);
    }else if(txt == "!2"){
      beep(2);
    }else{
      display.clear();
      display.drawRect(0,0,128,14);
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.setFont(ArialMT_Plain_10);
      display.drawString(64, 1, "BigStream Notice");
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.setFont(ArialMT_Plain_16);
      display.drawStringMaxWidth(64, 20,128, txt );
      display.display();
      display.displayOn();
      disp=10;
    }
}


void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) 
{
    Serial.println("Connected to NETPIE...");
    microgear.setAlias(ALIAS);
    microgear.subscribe(Topic);
}

void setup() 
{
    pinMode(D5, OUTPUT);
    display.init();
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 0, "> Starting");
    display.display();
     /* Event listener */
    microgear.on(MESSAGE,onMsghandler);
    microgear.on(CONNECTED,onConnected);

    Serial.begin(115200);
    Serial.println("Starting...");

    display.drawString(0, 12, "> Connecting Wifi...");
    display.display();
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
       delay(250);
       Serial.print(".");
    }
    display.drawString(0, 24, "> Connected");
    display.display();
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    microgear.init(KEY,SECRET,ALIAS);
    microgear.connect(APPID);
}

void loop() 
{
    delay(1500);
    if (microgear.connected())
    {
       microgear.loop();
       Serial.println("netpie connected");
    }
   else 
   {
    Serial.println("connection lost, reconnect...");
    microgear.connect(APPID);
   }

   if(disp>0){
      display.displayOn();
      disp = disp - 1;
    }else{
      display.displayOff();
    }
}
