/*    Arduino Long Range Wireless Communication using HC-12
                      Example 01
   by Dejan Nedelkovski, www.HowToMechatronics.com
*/
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

OneWire pino(10);
DallasTemperature barramento(&pino);
DeviceAddress sensor1,sensor2, sensor3, sensor4, sensor5, sensor6, sensor7, sensor8, sensor9, sensor10;

void setup() {
  Serial.begin(9600);             // Serial port to computer
  barramento.begin();
  barramento.getAddress(sensor1, 0);
  barramento.getAddress(sensor2, 1);
  barramento.getAddress(sensor3, 2);
  barramento.getAddress(sensor4, 3);
  barramento.getAddress(sensor5, 4);
  barramento.getAddress(sensor6, 5);
  barramento.getAddress(sensor7, 6);
  barramento.getAddress(sensor8, 7);
  barramento.getAddress(sensor9, 8);
  barramento.getAddress(sensor10, 9);
  
}
void loop() {

    barramento.requestTemperatures();
    delay(500);
    float temperatura [10];
    temperatura[0] = barramento.getTempC(sensor1);
    temperatura[1]  = barramento.getTempC(sensor2);
    temperatura[2] = barramento.getTempC(sensor3);
    temperatura[3] = barramento.getTempC(sensor4);
    temperatura[4] = barramento.getTempC(sensor5);
    temperatura[5] = barramento.getTempC(sensor6);
    temperatura[6] = barramento.getTempC(sensor7);
    temperatura[7] = barramento.getTempC(sensor8);
    temperatura[8] = barramento.getTempC(sensor9);
    temperatura[9] = barramento.getTempC(sensor10);

    
      
    delay(1000);

      for (int i = 0; i<10; i++){
        String sendData = "#";
        if (i < 10){
          sendData = sendData + temperatura[i] + ";";
          sendData = sendData + "temperatura" + (i+10) + ";"; // alterar index para i+10 para segundo transmiter
          sendData = sendData + "Temp" + "#";
        }
        Serial.println(sendData);
        delay(3000);
       }
      // If Serial monitor has data

   }
