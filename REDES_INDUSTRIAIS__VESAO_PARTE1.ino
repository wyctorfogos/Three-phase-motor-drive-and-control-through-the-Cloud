#define dht_apin 11 // Analog Pin sensor is connected to
#include <dht11.h>
dht11 dhtObject;

// Declação das saídas/relés
int rele_1=0; // Entrada digital para o relé 1
int rele_2=1; // Entrada digital para o relé 2
int rele_3=2; // Entrada digital para o relé 3
int rele_4=5; // Entrada digital para o relé 4
int rele_5=5; // Entrada digital para o relé 5

// Initialize our values
int number1 = 0;
int number2 = 0;
String comando;
int auxiliar=0;

void setup() {
  Serial.begin(9600);  
  pinMode(rele_1, OUTPUT); // Declaração das saídas digitais
  pinMode(rele_2, OUTPUT);
  pinMode(rele_3, OUTPUT);
  pinMode(rele_4, OUTPUT);
  pinMode(rele_5, OUTPUT);
}

void loop() {
  // Verificação das variáveis//
  dhtObject.read(dht_apin);
  number1 = dhtObject.temperature;
  number2 = dhtObject.humidity;
  Serial.print(" Temperatura (°C)= ");
  Serial.println(number1); 
  Serial.print(" Humidade in %= ");
  Serial.println(number2);
  Serial.print("");
  Serial.print(auxiliar);
  Serial.println("\n");

  if(Serial.available()>0){
    comando=Serial.readString();
    Serial.print(comando);
    if(comando=="STOP"){
      stop();}
    if(comando=="LIGAH"){
      starthorario();
       }
    if(comando=="LIGAA"){
      startantihorario();
    }
  }
  delay(1000);
}

///Essas funções são acionadas pelo serial///

void starthorario(){
digitalWrite(rele_5, HIGH);
digitalWrite(rele_4, HIGH);
digitalWrite(rele_3, HIGH);
digitalWrite(rele_2, LOW);
digitalWrite(rele_1, LOW);
auxiliar=1;
Serial.print("\n");
Serial.println("Sentido horario");
}
void stop(){
digitalWrite(rele_5, LOW);
digitalWrite(rele_4, LOW);
digitalWrite(rele_3, LOW);
digitalWrite(rele_2, LOW);
digitalWrite(rele_1, LOW);
auxiliar=0;
Serial.print("\n");
Serial.println("Parado");
}
void startantihorario(){
digitalWrite(rele_5, HIGH);
digitalWrite(rele_4, HIGH);
digitalWrite(rele_3, HIGH);
digitalWrite(rele_2, LOW);
digitalWrite(rele_1, LOW);
Serial.print("\n");
Serial.println("Sentido anti-horario") ;
auxiliar=2;
}
