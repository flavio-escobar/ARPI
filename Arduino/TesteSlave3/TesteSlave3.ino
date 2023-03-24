#include <Wire.h>
#include "EEPROMAnything.h"
#include <EEPROM.h>

int number = 0;
int state = 0;
int address = 0;
int nbr = 0;

struct config_t{
  int writed;
  int address;
  int number;
  //int cont;
}configuration;

void getValues(){
  Serial.print("\n------------------------------------------------\n");
  Serial.println("Obtendo Valores...");
  delay(200);
  EEPROM_readAnything(0, configuration);
  Serial.print("Gravado(1- sim, 0- nao):");
  Serial.println(configuration.writed);
  Serial.print("Endereco guardado:");
  Serial.println(configuration.address);
  Serial.print("Numero guardado:");
  Serial.println(configuration.number);
  
  if(configuration.writed == 1){
    address = configuration.address;
    number = configuration.number;
  Serial.println("Dados Recuperados!!");
  }else{
    address = 0;
    delay(300);
  }
  Serial.println("Feito!");
  Serial.print("\n------------------------------------------------\n");

}

boolean saveValues(){
  Serial.print("\n------------------------------------------------\n");
  Serial.println("Salvando Valores...");
  configuration.writed = 1;
  configuration.address = address;
  configuration.number = number;
  //configuration.cont = cont;
  
  EEPROM_writeAnything(0, configuration);
  
  Serial.print("Writed: ");
  Serial.println(configuration.writed);
  Serial.print("Endereco gravado: ");
  Serial.println(configuration.address);
  Serial.print("Numero: ");
  Serial.println(configuration.number);
  
  //EEPROM_writeAnything(0, configuration);
  Serial.println("Feito");
  Serial.print("\n------------------------------------------------\n");
}

void setup() {
  
  int botao = 40;
  int led_in = 13;
  pinMode(botao, INPUT);
  pinMode(led_in, OUTPUT);
  Serial.begin(9600);         // start serial for output
 
  getValues();
  //saveValues();
  
  //initialize i2c as slave
  Wire.begin(address);

  //define callbacks for i2c communication
  Wire.onReceive(receiveData);
  
  
  Serial.println("Ready!");
}

void loop() {
  interrupcao();
  delay(200);
}

void interrupcao(){
  int pwm = 8;
  int estadoBotao = 0;
  estadoBotao = digitalRead(40);
  
  if(estadoBotao == HIGH){
    analogWrite(pwm, 0);
    digitalWrite(13, HIGH);
  }else{
    digitalWrite(13, LOW);
    analogWrite(pwm, 255);
  }

}

//MODULOS PROGRAMAVEIS\\

/*
  -Modulo I-
Reenderecamento
*/
void receiveAddr(int bytCount) {
  
  int aux = 0;
  
  if (Wire.available()) {
    aux = Wire.read();
    address = aux;
    
    Serial.print("Dado Recebido: ");
    Serial.println(aux);
    saveValues(); 
    asm volatile ("  jmp 0");
  }
}

void sender(){
  
  //Constantes
  int MASTER_ADDRESS = 0;
  int SPCE = 4;
  int trash = 0;
  
  if(Wire.available()){
    trash = Wire.read();
    Wire.beginTransmission(MASTER_ADDRESS);
    delay(100);
    Wire.write(SPCE);
    Wire.endTransmission();
  }
  
}

/*
-Modulo II-
Comunicação
*/
void receiveInf(int bytCount) {
  int aux = 0;
  
  if (Wire.available()) {
    aux = Wire.read();
    if(aux != 255){
      number = aux;
    }
    
    Serial.print("\n*receiveInf*Dado Recebido: ");
    Serial.println(aux);
    saveValues(); 
  }
}

// callback for received data
void receiveData(int byteCount) {
  
  int aux = 0;
  
  if(Wire.available()) {
    aux = Wire.read();
    Serial.print("Dado Recebido: ");
    Serial.println(aux);
    
    if(aux == 1){
      nbr = aux;
      Serial.print("\n-----------------REENDERECANDO------------------\n");
      Wire.onRequest(sendDataR);
      Wire.onReceive(receiveAddr);
    }else if(aux == 2){
      Wire.onRequest(sendDataN);
    }else if(aux == 3){
      nbr = aux;
      Wire.onRequest(sendDataR);
      Wire.onReceive(receiveInf);
    }else{
      nbr = aux;
      Wire.onRequest(sendDataR);
    }
    aux = 0;
  }
}
// callback for sending data
void sendDataN() {
  Wire.write(number);
}

void sendDataR() {
  Wire.write(nbr);
}


