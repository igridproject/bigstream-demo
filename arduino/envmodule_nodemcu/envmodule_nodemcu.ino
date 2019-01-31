#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>

#define LightIn A0
#define LEDSTAT D4
#define WaitForWifi false
DHT dht;

const char* ssid = "BS-IOT";
const char* password = "1234567890";

int analoginValue,dI;
float volts;
void setup()
{
  dI=0;

  pinMode(LEDSTAT, OUTPUT);
  Serial.begin(9600);
  dht.setup(D2);

  WiFi.begin(ssid, password);

  if(WaitForWifi)
  {
    Serial.print("STA\tConnecting");
    while (WiFi.status() != WL_CONNECTED) {
   
      delay(1000);
      Serial.print(".");
   
    }
    Serial.println("\tOK");
  }
}

void loop() 
{
  delay(1000);
  wifiStatus();
  
  dI = dI + 1;
  if(dI>=9){

    analoginValue = analogRead(LightIn);
    //volts = (((float)analoginValue / 1024) * 5);
    int LPercent = map(analoginValue, 0, 1023, 0, 100); 

    float humidity = dht.getHumidity(); // ดึงค่าความชื้น
    float temperature = dht.getTemperature(); // ดึงค่าอุณหภูมิ
    String dhtstat = dht.getStatusString();


    if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
      HTTPClient http;  //Declare an object of class HTTPClient

      String data = "?temp=" + String(temperature,1) + "&hum=" + String(humidity,1) + "&light=" + String(LPercent) + "&dev=nodemcu01"; 
      http.begin("http://demo.bs.igridproject.info/io/demo.nodemcu01" + data);  //Specify request destination
      int httpCode = http.GET();
   
      if (httpCode > 0) { //Check the returning code
        String payload = http.getString();   //Get the request response payload
        sendingBlink();
      }else{delay(2000);}
   
      http.end();   //Close connection
   
    }else{
      delay(2000);
    }
  
    Serial.print(dhtstat);
    Serial.print("\t");
    Serial.print(humidity, 1);
    Serial.print("\t");
    Serial.print(temperature, 1);
    Serial.print("\t");
    Serial.println(LPercent);
    dI=0;
  }
  
}

void wifiStatus()
{
  if (WiFi.status() == WL_CONNECTED){
    digitalWrite(LEDSTAT, HIGH);
    delay(1000);
  }else{
    digitalWrite(LEDSTAT, HIGH);
    delay(1000);
    digitalWrite(LEDSTAT,LOW);
  }
}

void sendingBlink()
{
  digitalWrite(LEDSTAT, LOW);
  delay(100);
  digitalWrite(LEDSTAT, HIGH);
  delay(1900);
}

