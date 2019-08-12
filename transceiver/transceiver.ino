/*    Arduino Long Range Wireless Communication using HC-12
                      Example 01
   by Dejan Nedelkovski, www.HowToMechatronics.com
*/
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

SoftwareSerial HC12(11, 10); // HC-12 TX Pin, HC-12 RX Pin
OneWire pino(3);
DallasTemperature barramento(&pino);
DeviceAddress sensor1,sensor2, sensor3, sensor4, sensor5, sensor6, sensor7, sensor8, sensor9, sensor10;

void setup() {
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);               // Serial port to HC12
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
   String temp = "";
   while(HC12.available()){
       temp = temp + char(HC12.read());
   }
   if (temp == "!2"){ // alterar para 2 no segundo transmiter
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

    int umidade [10];
    umidade[0] = analogRead(A1);
    umidade[1] = analogRead(A2);
    umidade[2] = analogRead(A3);
    umidade[3] = analogRead(A4);
    umidade[4] = analogRead(A5);
    umidade[5] = analogRead(A6);
    umidade[6] = analogRead(A7);
    umidade[7] = analogRead(A8);
    umidade[8] = analogRead(A9);
    umidade[9] = analogRead(A10);
    
      
    delay(1000);

      for (int i = 0; i<20; i++){
        String sendData = "#";
        if (i < 10){
          sendData = sendData + temperatura[i] + ";";
          sendData = sendData + "temperatura" + (i+10) + ";"; // alterar index para i+10 para segundo transmiter
          sendData = sendData + "Temp" + "#";
        }
        else{
          sendData = sendData + umidade[i] + ";";
          sendData = sendData + "umidade" + (i) + ";"; // alterar index para i+10 para segundo transmiter
          sendData = sendData + "Umid" + "#";
        }
        HC12.print(sendData);
        delay(3000);
       }
      // If Serial monitor has data

   }

}
