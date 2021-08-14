#include <heltec.h>
#include <WiFi.h>
#include "ThingSpeak.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 17
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

#define Sensor1 36
#define Sensor2 37
#define Sensor3 38
#define Sensor4 39

int counter = 0;
int tempC = 0.0;
int sensor1 = 1, sensor2 = 1, sensor3 = 1, sensor4 = 1;
int waterLevel = 0;

char ssid[] = "###############";   
char pass[] = "y##############";   
const char * myWriteAPIKey = "##############"; 
const char* server = "api.thingspeak.com"; 
unsigned long myChannelNumber = ###########;          
WiFiClient  client; 


void logo(){ 
  Heltec.display->clear(); 
  Heltec.display->drawString(0, 0, "FATEC Pompeia");
  Heltec.display->drawString(0, 20, "BIG DATA");
  Heltec.display->drawString(0, 30, "No Agronegócio");
  Heltec.display->display();
} 

void water()
{                       
  pinMode(Sensor1, INPUT);                
  pinMode(Sensor2, INPUT);                
  pinMode(Sensor3, INPUT);                
  pinMode(Sensor4, INPUT);                
  int sensor1 = digitalRead(Sensor1);     
  int sensor2 = digitalRead(Sensor2);     
  int sensor3 = digitalRead(Sensor3);     
  int sensor4 = digitalRead(Sensor4);     
  Serial.print ("Sensor 1 : ");           
  Serial.println (sensor1);
  delay (1000);
  Serial.print ("Sensor 2 : ");           
  Serial.println (sensor2);
  delay (1000);
  Serial.print ("Sensor 3 : ");           
  Serial.println (sensor3);
  delay (1000);
  Serial.print ("Sensor 4 : ");           
  Serial.println (sensor4);
  delay (1000);
  
  
       if ( (sensor1 == 1)&& (sensor2 == 1)&& (sensor3 == 1)&& (sensor4 == 1)) {         
    //Serial.println (" Transbordamento!!! "); 
    waterLevel = 110;     
    delay (1000);
  }
  else if ( (sensor1 == 0)&& (sensor2 == 0)&& (sensor3 == 0)&& (sensor4 == 0)) {     
    //Serial.print ("Reservatório vazio!!!");
    waterLevel = 0;    
    delay (1000);
  }
  else if ( (sensor1 == 1)&& (sensor2 == 0)&& (sensor3 == 0)&& (sensor4 == 0)) {  
    //Serial.println ("Nível à 25%"); 
    waterLevel = 25;     
    delay (1000);
  }
   else if ( (sensor1 == 1)&& (sensor2 == 1)&& (sensor3 == 0)&& (sensor4 == 0)) {
    //Serial.println ("Nível à 50%"); 
    waterLevel = 50;   
    delay (1000);
  }
   else if ( (sensor1 == 1)&& (sensor2 == 1)&& (sensor3 == 1)&& (sensor4 == 0)) {    
    //Serial.println ("Nível à 100%"); 
    waterLevel = 100;   
    delay (1000);    
  }
  else{
  Serial.println ("Defeito!!!"); 
  waterLevel = 13;      
  delay (1000);
  }    
}





void setup(void)
{
  Serial.begin(115200);
  Serial.println("Leitura de temperatura iniciada.");  
  
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);  
  Heltec.display->init();  
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0, 0, "Leitura iniciada");
  delay(1500);
  Heltec.display->clear();
  WiFi.begin();   
  ThingSpeak.begin(client);  
  Serial.println("Sensor Temperatura inicializado");  
  sensors.begin();

  if(WiFi.status() != WL_CONNECTED){
    Heltec.display->drawString(0, 0, "Estabeleçendo conexão");
    Heltec.display->drawString(0, 10, "IP address:");
    Heltec.display->drawString(0, 20, String(WiFi.localIP()));
    Heltec.display->display();
    delay(1000);
    Heltec.display->clear();
   
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Heltec.display->drawString(0, 0, "...");
    Heltec.display->display();
    delay(1000);
    Heltec.display->clear();
  }
  Heltec.display->drawString(0, 0, "WiFi Conectado");
  Heltec.display->drawString(0, 10, "IP address:");
  Heltec.display->drawString(0, 20, String(WiFi.localIP()));
  Heltec.display->display();
  delay(1000); 
  }   
}


void loop(void)
{   
  Serial.print("Requerindo temperatura...");
  sensors.requestTemperatures(); 
  Serial.println("Encontrado"); 
  int tempC = sensors.getTempCByIndex(0); 
  if(tempC != DEVICE_DISCONNECTED_C) 
  {
    Serial.print("Temperatura: ");
    Serial.println(tempC);
  } 
  else
  {
    Serial.println("Erro");
  }

  water();

  
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "--Messages--"); 
  Heltec.display->drawString(0, 20, "Temp:");  
  Heltec.display->drawString(45, 20, String(tempC));
  Heltec.display->drawString(0, 30, "Water:");  
  Heltec.display->drawString(45, 30, String(waterLevel));   
  Heltec.display->display();


  Serial.println (tempC);
  Serial.println (waterLevel);

  counter++;                                        
  if(counter > 288){counter = 0;}  
  digitalWrite(25, HIGH);   
  delay(1000);                       
  digitalWrite(25, LOW); 
  delay(300000);  

  
  
  
  ThingSpeak.setField(1,tempC); 
  ThingSpeak.setField(2,waterLevel); 
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);    
    
 
  
  

  
  
}
