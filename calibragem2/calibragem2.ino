// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#define SSID "HM"
#define PW "121119978"
#define DHTPIN D2     // what digital pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE); // remover
float h, t, f; // remover

const String HASH = "e10ef52d9d001cae323ad472a23d6fb6";
const String API_PATH = "http://35.245.96.146/MoonProject/public/inserirDado";

String valor;
String tipoSensor;
String tag;

const int analogInPin1 = 0;  // Analog input pin that the potentiometer is attached to //remover
const int analogInPin2 = 1;  // Analog input pin that the potentiometer is attached to
const int analogInPin3 = 2;  // Analog input pin that the potentiometer is attached to
const int analogInPin4 = 3;  // Analog input pin that the potentiometer is attached to

int sensorReading[4];

void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  WiFi.begin(SSID, PW);   //WiFi connection
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(500);
    Serial.println("Waiting for connection");
 
  }

  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(60000);

  // função para realizar a leitura da temperatura
  lerTemperatura(); // alterar para receber dados via rádio

  tipoSensor = "analog";
  tag = "DHT";


  sensorReading[0] = analogRead(analogInPin1);// irão receber os dados via rádio
  sensorReading[1] = analogRead(analogInPin2);
  sensorReading[2] = analogRead(analogInPin3);
  sensorReading[3] = analogRead(analogInPin4);

  for (int i = 0; i < 4; i++){
    valor = String(sensorReading[i]);
    tag = "Sensor"+String(i);
    enviarAPI(tipoSensor, valor, tag);
    delay(1000);
  }
  
  

  
}

void enviarAPI(String tipoSensor, String valor, String tag){
    if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
  
     String postData = "hashDevice=" + HASH + "&valor=" + valor + "&tipoSensor=" + tipoSensor + "&tag=" + tag ;
     HTTPClient http;    //Declare object of class HTTPClient
     http.begin(API_PATH);      //Specify request destination
     http.addHeader("Content-Type", "application/x-www-form-urlencoded");
   
     int httpCode = http.POST(postData);   //Send the request
     String payload = http.getString();                  //Get the response payload
   
     Serial.println(httpCode);   //Print HTTP return code
     Serial.println(payload);    //Print request response payload
   
     http.end();  //Close connection
   
   }else{
   
      Serial.println("Error in WiFi connection");   
   
   }
  
 }

void lerTemperatura(){
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
  }
