/*    Arduino Long Range Wireless Communication using HC-12
                      Example 01
   by Dejan Nedelkovski, www.HowToMechatronics.com
*/
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "HM";  //ENTER YOUR WIFI SETTINGS
const char *password = "121119978";
const char *host = "35.245.96.146";

SoftwareSerial HC12(D5, D6); // HC-12 TX Pin, HC-12 RX Pin
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);               // Serial port to HC12
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}

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

void loop() {
  // solicita os dados de um sensor a cada X segundos
  unsigned long currentMillis = millis();
  unsigned long Switch = 0; // alterna entre os dois emissores
  if (currentMillis - previousMillis >= 60000) {
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

      // verifica se o wifi está conectado periodicamente
      if(WiFi.status() != WL_CONNECTED){
        Serial.println("Conexão perdida, reiniciando");
        ESP.restart();
      }
      
      sendPost(val, valType,tag);
      // metodo POST para envio das variaveis;
    }
      
   }
}
