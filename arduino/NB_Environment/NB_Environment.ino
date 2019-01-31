#include "AIS_NB_BC95.h"
#include "DHT.h"

#define DHTTYPE DHT11
#define DHTPIN 2 
#define LightIn A0


String apnName = "devkit.nb";

//BigStream Config
String serverIP = "203.185.71.24"; // Your Server IP
String serverPort = "19150"; // Your Server Port
String bsKey = "<VONAME>:nbenv"; // BigStream Job KeyName ( VONAME:KEYNAME )
String udpData = "";

AIS_NB_BC95 AISnb;

const long interval = 20000;  //millisecond
unsigned long previousMillis = 0;

long cnt = 0;

DHT dht(DHTPIN, DHTTYPE);
void setup()
{ 
  AISnb.debug = true;
  
  Serial.begin(9600);
  dht.begin();
 
  AISnb.setupDevice(serverPort);

  String ip1 = AISnb.getDeviceIP();  
  delay(1000);
  
  previousMillis = millis();

}
void loop()
{ 
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
    {
      cnt++;     

      //DHT11
      float hum = dht.readHumidity();
      float temp = dht.readTemperature();
      
      //Light Sensor
      int analoginValue = analogRead(LightIn);
      float volts = ((analoginValue / 1023) * 5);
      int lPercent = map(analoginValue, 0, 1023, 0, 100);

      Serial.print(hum, 1);
      Serial.print("\t");
      Serial.print(temp, 1);
      Serial.print("\t");
      Serial.println(lPercent);
      
      udpData = String(hum,1) + "," + String(temp,1) + "," + String(lPercent);
      
      // Send data in String 
      String dataPackage = bsKey + "," + udpData;
      UDPSend udp = AISnb.sendUDPmsgStr(serverIP, serverPort, dataPackage);
   

      previousMillis = currentMillis;
  
    }
  UDPReceive resp = AISnb.waitResponse();
     
}



