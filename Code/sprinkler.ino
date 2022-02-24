#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "DHT.h"


////////////////////////////////////// Login details ///////////////////////////////////////////////////////////////
#define WLAN_SSID      "robot"    
#define WLAN_PASS      "12345678"
#define MQTT_SERV      "io.adafruit.com"
#define MQTT_PORT       1883           
#define MQTT_NAME      "sreelekhagoud"
#define MQTT_PASS      "aio_cWBZ88uIrsjzahPqrFS60D63u8X9"
////////////////////////////////// Feeds  ///////////////////////////////////////////////////////////////////////////
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);
Adafruit_MQTT_Publish TEMPFeed = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/TEMP");
Adafruit_MQTT_Publish HUMFeed = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/HUM");
Adafruit_MQTT_Publish SOILFeed = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/SOIL");
Adafruit_MQTT_Subscribe IN = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/IN");
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char *VALUE;
String TIME,DIRECTION;
#define MOTOR1 5
#define MOTOR11 18
#define MOTOR2 21
#define MOTOR22 19
#define SOIL_SENSOR 35

#define DHTPIN 13
#define WATER_PUMP 26
#define BUZZER 15
#define LIGHT 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
int  TEMP,HUM;
int SOIL_VALUE;
void setup()
{
  Serial.begin(9600);
  pinMode(MOTOR1,OUTPUT);
  pinMode(MOTOR11,OUTPUT);
  pinMode(MOTOR2,OUTPUT);
  pinMode(MOTOR22,OUTPUT);
  pinMode(BUZZER,OUTPUT);
  pinMode(LIGHT,OUTPUT);
  pinMode(WATER_PUMP,OUTPUT);
  pinMode(SOIL_SENSOR,INPUT);
  digitalWrite(WATER_PUMP,HIGH);



  dht.begin();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.print(WiFi.localIP());
  mqtt.subscribe(&IN);
  MQTT_connect();
}
void FORWARD(){

  digitalWrite(MOTOR1,HIGH);
  digitalWrite(MOTOR11,LOW);
  digitalWrite(MOTOR2,HIGH);
  digitalWrite(MOTOR22,LOW);
  Serial.println("FORWARD");
}
void BACKWARD(){

  digitalWrite(MOTOR1,LOW);
  digitalWrite(MOTOR11,HIGH);
  digitalWrite(MOTOR2,LOW);
  digitalWrite(MOTOR22,HIGH);
  Serial.println("BACKWARD");

}
void RIGHT(){

  digitalWrite(MOTOR1,HIGH);
  digitalWrite(MOTOR11,LOW);
  digitalWrite(MOTOR2,LOW);
  digitalWrite(MOTOR22,HIGH);
  Serial.println("RIGHT");

}
void LEFT(){


  digitalWrite(MOTOR1,LOW);
  digitalWrite(MOTOR11,HIGH);
  digitalWrite(MOTOR2,HIGH);
  digitalWrite(MOTOR22,LOW);
  Serial.println("LEFT");

}
void STOP(){
  digitalWrite(MOTOR1,LOW);
  digitalWrite(MOTOR11,LOW);
  digitalWrite(MOTOR2,LOW);
  digitalWrite(MOTOR22,LOW);
  Serial.println("STOP");

}
void loop() { 
  TEMP = dht.readTemperature();
  HUM = dht.readHumidity();
  SOIL_VALUE = analogRead(SOIL_SENSOR);
  Serial.print("TEMP: ");
  Serial.println(TEMP);
  Serial.print("HUM: ");
  Serial.println(HUM);
  Serial.print("SOIL_VALUE:  ");
  Serial.println(SOIL_VALUE);  

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (! SOILFeed.publish(SOIL_VALUE))
  {
    Serial.println(F("Failed"));
  }
 
  else
  {
    Serial.println(F("OK!"));
    delay(2000);
  }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   if (! HUMFeed.publish(HUM))
  {
    Serial.println(F("Failed"));
  }
 
  else
  {
    Serial.println(F("OK!"));
    delay(2000);
  }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 if (! TEMPFeed.publish(TEMP))
  {
    Serial.println(F("Failed"));
  }
 
  else
  {
    Serial.println(F("OK!"));
    delay(2000);
  }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    if (subscription == &IN)
    {
      VALUE = (char*) IN.lastread;
      Serial.println(VALUE);
    }
    
    String INPUT_VALUE  = VALUE;
    TIME = INPUT_VALUE.substring( 0, INPUT_VALUE.indexOf(","));
    DIRECTION = INPUT_VALUE.substring(INPUT_VALUE.indexOf(",")+1, INPUT_VALUE.length());
    Serial.println(TIME);
    Serial.println(DIRECTION);
    int Number = TIME.toInt();
///////////////////////////////////////////////////////////////////////////////
    if(DIRECTION == "B")
    {
    for (int i = 0;i<Number*10;i++){
      LEFT();
      }
      STOP(); 
      if(SOIL_VALUE > 3000){
      digitalWrite(WATER_PUMP,LOW);
      digitalWrite(BUZZER,HIGH);
      digitalWrite(LIGHT,HIGH);
      delay(2000);
      digitalWrite(WATER_PUMP,HIGH);
      digitalWrite(BUZZER,LOW);
      digitalWrite(LIGHT,LOW);
      }
    }
/////////////////////////////////////////////////////////////////////////////////
    if(DIRECTION == "F")
    {
    for (int i = 0;i<Number*10;i++){
      RIGHT();
      }
      STOP();
            if(SOIL_VALUE > 3000){
      digitalWrite(WATER_PUMP,LOW);
      digitalWrite(BUZZER,HIGH);
      digitalWrite(LIGHT,HIGH);
      delay(2000);
      digitalWrite(WATER_PUMP,HIGH);
      digitalWrite(BUZZER,LOW);
      digitalWrite(LIGHT,LOW);
    }
    }
///////////////////////////////////////////////////////////////////////////////////////
    if(DIRECTION == "R")
    {
    for (int i = 0;i<Number*10;i++){
      FORWARD();
      }
     STOP();
           if(SOIL_VALUE > 3000){

      digitalWrite(WATER_PUMP,LOW);
      digitalWrite(BUZZER,HIGH);
      digitalWrite(LIGHT,HIGH);
      delay(2000);
      digitalWrite(WATER_PUMP,HIGH);
      digitalWrite(BUZZER,LOW);
      digitalWrite(LIGHT,LOW);
    }
    }
/////////////////////////////////////////////////////////////////////////////////////////
   if(DIRECTION == "L")
    {
    for (int i = 0;i<Number*10;i++){
     BACKWARD();
      }
     STOP();
           if(SOIL_VALUE > 3000){

      digitalWrite(WATER_PUMP,LOW);
      digitalWrite(BUZZER,HIGH);
      digitalWrite(LIGHT,HIGH);
      delay(2000);
      digitalWrite(WATER_PUMP,HIGH);
      digitalWrite(BUZZER,LOW);
      digitalWrite(LIGHT,LOW);
    }
    } 
   
}
}
void MQTT_connect()
{
  int8_t ret;
  if (mqtt.connected())
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0)
    {
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);
       retries--;
       if (retries == 0)
       {
         while (1);
       }
    }
  Serial.println("MQTT Connected!");
}
