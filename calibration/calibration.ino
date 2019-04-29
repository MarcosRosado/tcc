/*********************************************************************
  This is an example for reading the analog sensor output of a
  capacitive soil moisture probe and displaying the result on a
  128x64 OLED display over the I2C bus.
  This example requires the SSD1306 display driver by Adafruit:
  https://github.com/adafruit/Adafruit_SSD1306
  Video detailing the capacitive soil moisture measurement probe:
  https://youtu.be/pdGRs7GXBeE
*********************************************************************/

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// make sure the SSD1306 driver is configured for 64 line height
#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

const int wetProbe      = 300;   // wet readings are around 1.5v or analog input value ~300
const int dryProbe      = 630;   // dry readings are around   3v or analog input value ~620

const int analogInPin = A3;  // Analog input pin that the potentiometer is attached to


// Analog Sensor Input Pin
int sensorInput         = 0;    // soil probe is on analog input 0

// Variables
int validSensorReading  = 0;    // valid sensor analog reading to record
int sensorReading;              // new sensor reading to evaluate and record or discard
int sensorResult;               // scaled sensor data [0..3] = [wet, damp, moist, dry]
int ldrValue = 0;        // Variavel DHT
//valores máximos e minimos do sensor
int minValue = 2000;
int maxValue = 0;

float h, t;

DHT dht(DHTPIN, DHTTYPE);

void setup()   {

  Serial.begin(9600);           // some debug messages will display sensor raw data
  dht.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C address of the display
  display.clearDisplay();                     // clear the display buffer
}


void loop() {

  leituraUmidade();
  //leituraDHT();
  verificaMaxMin();
  //leituraLDR();

  
  // display the correct soil moisture level on the display
  // lower voltages represent more wet levels
  displayTextProbe(sensorResult);
  delay(1500);
}

void leituraUmidade(){
   // compara a leitura do sensor com a ultima para garantir estabilidade e evitar erros
  sensorReading = analogRead(sensorInput);
  if (abs(validSensorReading - sensorReading) > 5) {
    validSensorReading = sensorReading;
  }

  // console debug
  Serial.print ("Old Sensor Reading: ");
  Serial.println (validSensorReading);
  Serial.print ("New Sensor Reading: ");
  Serial.println (sensorReading);

  // conversão para escala de min max
  sensorResult = map(validSensorReading, wetProbe, dryProbe, 100, 0);  // scale analog input to a smaller range for wet to dry
  Serial.print ("Scaled Sensor Reading Percentage: ");
  Serial.print (sensorResult);
  Serial.println ("%");
  Serial.println ();
    
  }


void leituraLDR(){
  // realiza a leitura do LDR
  ldrValue = analogRead(analogInPin);
  }

void leituraDHT(){
  // realiza a leitura do DHT
  h = dht.readHumidity();
  t = dht.readTemperature();
  
  
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
}

void verificaMaxMin(){
  //verifica a máxima e minima do sensor
  if (sensorReading < minValue){
    minValue = sensorReading;
  }

  if (sensorReading > maxValue){
    maxValue = sensorReading;
  }

 
}

void displayTextProbe(const char * sensorDisplay) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.clearDisplay();
  display.print("Umidade cap: ");
  display.println(sensorReading);
  display.print("Maxima: ");
  display.println(maxValue);
  display.print("Minima: ");
  display.println(minValue);
  display.display();
  delay(1);
}
