/*    Arduino Long Range Wireless Communication using HC-12
                      Example 01
   by Dejan Nedelkovski, www.HowToMechatronics.com
*/
#include <SoftwareSerial.h>



SoftwareSerial HC12(5, 6); // HC-12 TX Pin, HC-12 RX Pin
unsigned long previousMillis = 0;
bool sendval = true;
unsigned long Switch = 0; // alterna entre os dois emissores

void setup() {
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);               // Serial port to HC12
  delay(1000);

}
/*
void sendPost(String valor, String tipo, String tag){
   HTTPClient http;
   String postData = "";
   postData = postData + "hashDevice=" + "808535c25bcce81e3904ac4d89f7ffed" + "&valor=" + valor + "&tipoSensor=" + tipo + "&tag=" + tag ;
   http.begin("http://35.245.96.146/MoonProject/public/inserirDado");              //Specify request destination
   http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
 
   int httpCode = http.POST(postData);   //Send the request
   String payload = http.getString();    //Get the response payload
 
   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload
 
  http.end();  //Close connection
  }
*/
void loop() {
  // solicita os dados de um sensor a cada X segundos
  unsigned long currentMillis = millis();
  unsigned long Switch = 0; // alterna entre os dois emissores
  delay(2000);
  if (sendval == true){
    HC12.print("!2");
    sendval = false;
    }
  if (currentMillis - previousMillis >= 120000) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    if (Switch % 2 == 0){
      HC12.print("!1");
    }
    else{
      HC12.print("!2");
    }
    delay(500);
    Switch ++;
  }

  // recebe os dados solicitados
  String temp = "";
  while (HC12.available()) {        // If HC-12 has data
    temp = temp + char(HC12.read());
  }
  
  delay(1000); // pequeno delay para evitar erros durante a recepção e o processamento dos dados
  
  if(temp != ""){ // caso a string esteja preenchida, processa os dados recebidos.
    int len = temp.length();
    if(temp[0] == '#' && temp[len-1] == '#'){ // verifica se a string foi recebida integralmente
      temp.remove(len-1, 1); // remove o ultimo valor
      temp.remove(0,1); // remove o primeiro valor
      
      // realiza o processamento da string
      len = temp.length();
      String val = "";
      String tag = "";
      String valType = "";
      int variableCont = 0;
      
      for (int i = 0; i < len; i++){ // separa a string recebida nas 3 variaveis para envio
        if(temp[i] == ';'){ // verifica se eh o char de separação
            variableCont ++;
        }

        else{ // grava o valor na variavel correta
          if(variableCont == 0){
            val += temp[i];
          }
          if(variableCont == 1){
            tag += temp[i];
          }
          if(variableCont == 2){
            valType += temp[i];
          }
        }
      }
      Serial.print("val: ");
      Serial.println(val);
      Serial.print("valTaype: ");
      Serial.println(valType);
      Serial.print("tag: ");
      Serial.println(tag);
      delay(1000);
      //sendPost(val, valType,tag);
      // metodo POST para envio das variaveis;
    }
      
   }
}
