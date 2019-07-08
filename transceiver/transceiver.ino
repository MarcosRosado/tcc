/*    Arduino Long Range Wireless Communication using HC-12
                      Example 01
   by Dejan Nedelkovski, www.HowToMechatronics.com
*/
#include <SoftwareSerial.h>
SoftwareSerial HC12(11, 10); // HC-12 TX Pin, HC-12 RX Pin
void setup() {
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);               // Serial port to HC12
}
void loop() {
   String temp = "";
   while(HC12.available()){
       temp = temp + char(HC12.read());
   }
   if (temp == "!1"){
    delay(1000);

      for (int i = 0; i<40; i++){
        int randNum = random(0,1024);
        String sendData = "#";
        sendData = sendData + randNum + ";";
        sendData = sendData + "sensor" + i + ";";
        sendData = sendData + "analog" + "#";
        HC12.print(sendData);
        delay(2000);
       }
      // If Serial monitor has data

   }

}
