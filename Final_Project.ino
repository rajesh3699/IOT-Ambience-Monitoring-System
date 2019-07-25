#include <MQ135.h>
#include <DHT.h>
#include<FirebaseArduino.h>
#include<ESP8266WiFi.h>
#include<ArduinoJson.h>

//Setting wifi...
#define WIFI_SSID "open"
#define WIFI_PASSWORD "12345678"

//firebase info..

#define FIREBASE_HOST "iotproject2-70b1c.firebaseio.com"
#define FIREBASE_AUTH "ukCpwwHkiLUMN0ZUuaRGiqsdms2uXqrbCUZ3lwNR "
#define DHTPIN 2    
#define DHTTYPE DHT11
#define SOUNDPIN 15
 
const int MQ135PIN = A0;
DHT dht(DHTPIN, DHTTYPE);


MQ135 gasSensor = MQ135(MQ135PIN);
void setup(){
  Serial.begin(115200); // sets the serial port to 9600

  //setting wifi...
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
Serial.print("Connecting to Wi-Fi");
while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
 
  
  Serial.println(F("DHT11 test!"));
  dht.begin();

}
void loop(){
   float h = dht.readHumidity();
 
   float t = dht.readTemperature();

   float ppm = gasSensor.getPPM()*200000 +400;
  
  
   if (isnan(h) || isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    delay(3000);
    return;
   }

 //printing humidity to serial monitor
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print("% ");

  //sending humidity to firebase
  Firebase.setFloat("Humidity",h);
  if (Firebase.failed()) { 
     Serial.print("sending humidity failed:"); 
     Serial.println(Firebase.error());   
     return; 
 } 
 

  //printing temperature on serial monitor
  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.println(F("°C"));  

  //sending temperature to firebase
  Firebase.setFloat("Temperature",t);
  if (Firebase.failed()) { 
     Serial.print("sending temperature failed:"); 
     Serial.println(Firebase.error());   
     return; 
 } 
 
  
 //printing ppm to serial monitor.. 
  Serial.print(ppm);
  Serial.print("ppm   Airquality: ");

  //sending ppm to firebase..
  Firebase.setFloat("ppm",ppm);
  if (Firebase.failed()) { 
     Serial.print("sending ppm failed:"); 
     Serial.println(Firebase.error());   
     return; 
 } 
 
  
  
  if(ppm<1000){
   
  Serial.println("safe\n\n");
  Firebase.setString("Airquality","safe");
   if (Firebase.failed()) { 
     Serial.print("sending airquality failed:"); 
     Serial.println(Firebase.error());   
     return; 
 } 
     
    }
    
  else if (ppm<2000){
      
  Serial.println("bad\n\n ");
  Firebase.setString("Airquality","bad");    
   if (Firebase.failed()) { 
     Serial.print("sending airquality failed:"); 
     Serial.println(Firebase.error());   
     return; 
 } 
 
    
    }
      
  else{
  Serial.println("Harmful\n\n ");
  Firebase.setString("Airquality","harmful");
   if (Firebase.failed()) { 
     Serial.print("sending airquality failed:"); 
     Serial.println(Firebase.error());   
     return; 
 } 
 
    
        }

        
  
  
int  noisePollution=digitalRead(SOUNDPIN);

  
    if(noisePollution==1)
  { 
    Serial.println("SOUND KAM KAR");
    Firebase.setString("NOISE","harmful");
   if (Firebase.failed()) { 
     Serial.print("sending airquality failed:"); 
     Serial.println(Firebase.error());   
     return; 
 } 
   
    }
    else{
      Serial.println("SOUND BADA");
      Firebase.setString("NOISE","safe");
   if (Firebase.failed()) { 
     Serial.print("sending airquality failed:"); 
     Serial.println(Firebase.error());   
     return; 
 } 
      }
 
}
