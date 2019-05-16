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
#define OLED_RESET 4

Adafruit_SSD1306 display(OLED_RESET);

// make sure the SSD1306 driver is configured for 64 line height
#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

const int wetProbe      = 310;   // wet readings are around 1.5v or analog input value ~300
const int dryProbe      = 660;   // dry readings are around   3v or analog input value ~620

const int analogInPin1 = 0;  // Analog input pin that the potentiometer is attached to
const int analogInPin2 = 1;  // Analog input pin that the potentiometer is attached to
const int analogInPin3 = 2;  // Analog input pin that the potentiometer is attached to
const int analogInPin4 = 3;  // Analog input pin that the potentiometer is attached to


// Analog Sensor Input Pin
int sensorInput         = 0;    // soil probe is on analog input 0

// Variables
int validSensorReading  = 0;    // valid sensor analog reading to record
int sensorReading1;              // new sensor reading to evaluate and record or discard
int sensorReading2;              // new sensor reading to evaluate and record or discard
int sensorReading3;              // new sensor reading to evaluate and record or discard
int sensorReading4;              // new sensor reading to evaluate and record or discard

int sensorResult1;
int sensorResult2;
int sensorResult3;
int sensorResult4;

void setup()   {

  Serial.begin(9600);           // some debug messages will display sensor raw data
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C address of the display
  display.clearDisplay();                     // clear the display buffer
}


void loop() {

  leituraUmidade();
  
  // display the correct soil moisture level on the display
  // lower voltages represent more wet levels
  displayTextProbe();
  delay(1500);
}

void leituraUmidade(){
  sensorReading1 = analogRead(analogInPin1);
  sensorReading2 = analogRead(analogInPin2);
  sensorReading3 = analogRead(analogInPin3);
  sensorReading4 = analogRead(analogInPin4);

  /*
  sensorResult1 = map(sensorReading1, wetProbe, dryProbe, 320, 630);
  sensorResult2 = map(sensorReading2, wetProbe, dryProbe, 320, 630);
  sensorResult3 = map(sensorReading3, wetProbe, dryProbe, 320, 630);
  sensorResult4 = map(sensorReading4, wetProbe, dryProbe, 320, 630);
  */
  
  if (abs(sensorResult1 - sensorReading1) > 4) {
    sensorResult1 = sensorReading1;
  }
  if (abs(sensorResult2 - sensorReading2) > 4) {
    sensorResult2 = sensorReading2;
  }
  if (abs(sensorResult3 - sensorReading3) > 4) {
    sensorResult3 = sensorReading3;
  }
  if (abs(sensorResult4 - sensorReading4) > 4) {
    sensorResult4 = sensorReading4;
  }

  
  // console debug
  Serial.print ("New Sensor Reading1: ");
  Serial.println (sensorReading1);
  Serial.print ("New Sensor Reading2: ");
  Serial.println (sensorReading2);
  Serial.print ("New Sensor Reading3: ");
  Serial.println (sensorReading3);
  Serial.print ("New Sensor Reading4: ");
  Serial.println (sensorReading4);

    
  }


void displayTextProbe() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.clearDisplay();
  display.print("Sensor 1: ");
  display.println(sensorResult1);
  display.print("Sensor 2: ");
  display.println(sensorResult2);
  display.print("Sensor 3: ");
  display.println(sensorResult3);
  display.print("Sensor 4: ");
  display.println(sensorResult4);
  display.display();
  delay(1);
}
