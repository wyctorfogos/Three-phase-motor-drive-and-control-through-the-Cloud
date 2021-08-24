#include <ESP8266WiFi.h>
#include <DHT.h>
#include<ESP8266WebServer.h>
#include "ThingSpeak.h"
//Habilita pino D4 para envio de dados
#define DHTPIN D4 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11
#define rele_3 D6
#define rele_2 D2
#define rele_1 D1
#define rele_4 D0
#define rele_5 D5
//Define pino e tipo de DHT a ser utilizado
DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;   // make the client of the WiFi which connect to the ThingSpeak webServer
ESP8266WebServer server(80);    // make the naother server


unsigned long myChannelNumber = 1454411;
const char * myWriteAPIKey = "FLH8KHVWHTE0T5QA";
 
//Credenciais ThingSpeak
const char* host = "api.thingspeak.com";
String ApiKey = "FLH8KHVWHTE0T5QA";
 
//Caminho para leitura dos dados
String path_one = "/update?key=" +ApiKey + "&field1=";
String path_two = "&field2=";
 
//Login e senha do wi-fi principal
char* ssid = "Redmi Note 9S"; //coloque aqui o nome da rede principal que se deseja conectar
char* pass = "12345678"; //coloque aqui a senha da rede principal que se deseja conectar
 
//Define contador igual a zero
int count = 0;
String myStatus;
int auxiliar=0;
unsigned long pmillis=0;
unsigned long pmillis2=0;
const long intervalo=5000;
int comando=0;
int auxvelocidade=0;
int aux=0;

void setup()
{
  pinMode(rele_1, OUTPUT);
  pinMode(rele_2, OUTPUT);
  pinMode(rele_3, OUTPUT);
  pinMode(rele_4, OUTPUT);
  pinMode(rele_5, OUTPUT);
//Inicia DHT11
  Serial.println("Inicia teste no DHT11!");
  dht.begin();
  Serial.begin(115200);
  Serial.println("Estabelecendo conexão...");
  connect();
   
//Caso o Wi-Fi seja desconectado:
    if(WiFi.status()!=WL_CONNECTED)
      {
        Serial.print("A conexão não foi estabelecida, por favor apertar botão reset ou aguardar...\n");
        return;
      }     
}
void loop() 
{
  server.handleClient();
  comando=ThingSpeak.readFloatField(myChannelNumber,4);
  auxvelocidade=ThingSpeak.readFloatField(myChannelNumber,3);
 // A leitura da temperatura e umidade pode levar 250ms!
  // O atraso do sensor pode chegar a 2 segundos.
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  myStatus= Status_reles();
  // Testa se retorno é valido, caso contrário algo está errado.
  if (isnan(t) || isnan(h)) 
 {
    Serial.println("Falha na leitura do DHT");
 } 
  else
{
    if ((millis()-pmillis2)>intervalo){
    Serial.print("Umidade: ");
    Serial.println(h);
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" *C");
    pmillis2=millis();
    }
    
 //if(comando!=0){
  
      delay(50);
      Serial.print(comando);
         if(auxvelocidade==0){
        velocidade1(); 
        }
         if(auxvelocidade==1){
        velocidade2(); 
        }
         if(auxvelocidade==2){
        velocidade3(); 
        }
         if(auxvelocidade==3){
        velocidade4(); 
        }
      if (aux==0 || aux==2){
        if(comando==0){
          delay(50);
          stopmotor();
          aux=1;}}
       if (aux==1 || aux==2){    
        if(comando==1){
          starthorario();
          aux=0;}
         }
       if (aux==0 || aux==1){    
      if(comando==2){
        startantihorario();
        aux=2;}
      //}
   }
 }
//Conexão ao endereço IP específico
WiFiClient client;
const int httpPort = 80;
 
//Caso não se conecte ao ThingSpeak
if (!client.connect(host, httpPort))
  {
    Serial.println("Falha na conexão, por favor aguarde estabilização da rede");
    if (ssid=="LoginPrincipal") //coloque aqui o nome da rede principal que se deseja conectar
    {
      ssid = "Redmi Note 9S"; //coloque aqui o nome da rede auxiliar que se deseja conectar
      pass = "12345678"; //coloque aqui a senha da rede auxiliar que se deseja conectar
      Serial.println("Estabelecendo conexão na rede auxiliar");
      connect();
 
      //Tenta estabelecer nova conexão a rede principal
      if(WiFi.status()!=WL_CONNECTED)
      return;
    }
    else
    {
      ssid = "Redmi Note 9S"; //coloque aqui o nome da rede principal que se deseja conectar
      pass = "12345678"; //coloque aqui a senha da rede principal que se deseja conectar
      Serial.println("Estabelecendo conexão na rede principal");
      connect();
 
      //Tenta estabelecer nova conexão a rede auxiliar
      if(WiFi.status()!=WL_CONNECTED)
      return;
    }
    return;
  }
 
//Envia os dados de leitura do sensor DHT 11 ao ThingSpeak

       if ((millis()-pmillis)>intervalo){
         client.print(String("GET ") + path_one + t + path_two + h + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" + 
         "Connection: keep-alive\r\n\r\n");
         ThingSpeak.setStatus(myStatus);
         pmillis=millis();
 }
 }
void connect()
{
  //Quando conectado contador volta a zero
  count = 0;
  WiFi.begin(ssid, pass);
 
  //Enquanto Wi-Fi não estiver conectado e o limite do tempo de conexão não exceder
  while (WiFi.status()!= WL_CONNECTED && count<= 130)
  {
    delay(100);
    Serial.print(".");
    count = count + 1;
// start the server
  }
  if ((WiFi.status() == WL_CONNECTED))
  {
    Serial.println("");
    Serial.print("Conectado em: ");
    Serial.println(ssid);
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());
    server.on("/",handleonconnect);    // in urt type the "/" then call the handle on connect function
    ThingSpeak.begin(client);      // connect the client to the thingSpeak server
    server.begin();   
   if (ssid=="JAILZA") //coloque aqui o nome da rede principal que se deseja conectar
   {
      Serial.println("Rede principal conectada");
      delay (100);
     
    }
   if (ssid=="JAILZA") //coloque aqui o nome da rede auxiliar que se deseja conectar
   {
      Serial.println("Rede auxiliar conectada");
      delay(100);
   }
  }
}

String Status_reles(){
  // figure out the status message
  if(auxiliar==1){
    myStatus = String("Motor funcionando antihorario"); 
  }
  else if(auxiliar==2){
    myStatus = String("Motor funcionando sentido horario");
  }
  else{
    myStatus = String("Motor parado");
  }
  return String(myStatus);
}


///Essas funções são acionadas pelo serial///

void velocidade1(){
  digitalWrite(rele_2, LOW);
  digitalWrite(rele_1, LOW);
  
}
void velocidade2(){
  digitalWrite(rele_2, LOW);
  digitalWrite(rele_1, HIGH);
  
}
void velocidade3(){
  digitalWrite(rele_2, HIGH);
  digitalWrite(rele_1, LOW);
}
void velocidade4(){
  digitalWrite(rele_2, HIGH);
  digitalWrite(rele_1, HIGH);}
void starthorario(){

digitalWrite(rele_4, LOW);
digitalWrite(rele_3, HIGH);
auxiliar=1;

Serial.println("Sentido horario");
}
void stopmotor(){
digitalWrite(rele_4, HIGH);
digitalWrite(rele_3, HIGH);
auxiliar=0;

Serial.println("Parado");
}
void startantihorario(){

digitalWrite(rele_3, LOW);
digitalWrite(rele_4, HIGH);
Serial.println("Sentido anti-horario") ;
auxiliar=2;
}

void handleonconnect()
{
 server.send(200,"text/html",SendHTML());
}

String SendHTML(void){
 String ptr = "<!DOCTYPE html> <html>\n";
 ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
 ptr +="<title>LED Control</title>\n";
 ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
 ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
 ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
 ptr +=".button-on {background-color: #1abc9c;}\n";
 ptr +=".button-on:active {background-color: #16a085;}\n";
 ptr +=".button-off {background-color: #34495e;}\n";
 ptr +=".button-off:active {background-color: #2c3e50;}\n";
 ptr +=".button-aux {background-color: #34495e;}\n";
 ptr +=".button-aux:active {background-color: #2c3e50;}\n";
 ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
 ptr +="</style>\n";
 ptr +="</head>\n";
 ptr +="<body>\n";
 ptr +="<h1>Redes industriais </h1>\n";
 ptr +="<h3>Tarefa 3</h3>\n";
 ptr +="<h4>Controle e monitoramento de motor</h4>\n";
 ptr +="<a class=\"button button-aux\" href=\"https://api.thingspeak.com/update?api_key=FLH8KHVWHTE0T5QA&field4=2\">Sentido anti-Horario</a>\n";
 ptr +="<a class=\"button button-on\" href=\"https://api.thingspeak.com/update?api_key=FLH8KHVWHTE0T5QA&field4=1\">Sentido Horario</a>\n";
 ptr +="<a class=\"button button-off\" href=\"https://api.thingspeak.com/update?api_key=FLH8KHVWHTE0T5QA&field4=0\">Parado</a>\n";
 ptr +="</body>\n";
 ptr +="</html>\n";
 return ptr;

//07/08/2021
//String ptr="<head><meta name=\"viewport\"\n"; 
//ptr +="content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
//ptr +="<title>Motor Control</title>\n";
//ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
//ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
//ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
//ptr +=".button-on {background-color: #1abc9c;}\n";
//ptr +=".button-on:active {background-color: #16a085;}\n";
//ptr +=".button-off {background-color: #34495e;}\n";
//ptr +=".button-off:active {background-color: #2c3e50;}\n";
//ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
//ptr +="</style>\n";
//ptr +="</head>\n";
//ptr +="<body>\n";
//ptr +="<h1>ESP8266 with ThingSpeak Server</h1>\n";
//ptr +="<h3>Using Station(STA) Mode</h3>\n";
//ptr +="<h4>Control status For D1</h4>\n";
//ptr +="<button type='button' id='Ligar o Motor - Girar no Sentido Anti-Horário' class='Girar sentido anti-horário'>Sentido anti-Horario</button>\n";
//ptr +="<button type='button' id='Ligar o Motor - Girar no Sentido Horário' class='Girar sentido horário'>Sentido Horario</button>\n";
//ptr +="<button type='button' id='Desligar o Motor' class='Desligar o motor'>Parado</button>\n";  
//ptr +="<script>\n"; 
//ptr +="document.getElementById('Ligar o Motor - Girar no Sentido Horário').addEventListener('click', function() {\n";
//ptr +="var url = 'http://api.thingspeak.com/update?api_key='+'FLH8KHVWHTE0T5QA'+'&field4=1'\n";
//ptr +="$.getJSON(url, function(data) {\n";
//ptr +="console.log(data);\n";
//ptr +="});\n";
//ptr +="});\n";    
//ptr +="document.getElementById('Ligar o Motor - Girar no Sentido Anti-Horário').addEventListener('click', function() {\n";
//ptr +="var url = 'http://api.thingspeak.com/update?api_key='+'FLH8KHVWHTE0T5QA'+'&field4=-1'\n";
//ptr +="$.getJSON(url, function(data) {\n";
//ptr +="console.log(data);\n";
//ptr +="});\n";
//ptr +="});\n";
//ptr +="document.getElementById('Desligar o Motor').addEventListener('click', function() {\n";
//ptr +="var url = 'http://api.thingspeak.com/update?api_key='+'FLH8KHVWHTE0T5QA'+'&field4=0'\n";
//ptr +="$.getJSON(url, function(data) {\n";
//ptr +="console.log(data);\n";
//ptr +="});\n";
//ptr +="});\n";
//ptr +="setInterval(function() {\n";
//ptr +="// Call a function repetatively with 15 Second interval\n";
//ptr +="getThingSpeakLightState();\n";
//ptr +="}, 15000);\n";
//    
//ptr +="function getThingSpeakLightState() {\n";
//ptr +="var xhttp = new XMLHttpRequest();\n";
//ptr +="xhttp.open('GET', 'https://api.thingspeak.com/channels/1454411/fields/1/last.json', true);\n";
//ptr +="xhttp.send();\n";
//ptr +="}\n";
//ptr +="</script>\n";
//ptr +="</body>\n";
//ptr +="</html>\n";
//return ptr;
}
