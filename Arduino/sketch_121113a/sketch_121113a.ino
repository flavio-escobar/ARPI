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
  
  pinMode(13, OUTPUT);
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
  delay(100);
}

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

// callback for received data
void receiveData(int byteCount) {
  
  int aux = 0;
  
  if (Wire.available()) {
    aux = Wire.read();
    Serial.print("Dado Recebido: ");
    Serial.println(aux);
    
    if(aux == 1){
      nbr = aux;
      Serial.print("\n-----------------REENDERECANDO------------------\n");
      Wire.onRequest(sendDataR);
      Wire.onReceive(receiveAddr);
    }else if(aux == 2){
      getValues();
      delay(100);
      Wire.onRequest(sendDataN);
    }else{
      number = aux;
      saveValues();
      Wire.onRequest(sendDataN);
    }
  }
}
// callback for sending data
void sendDataN() {
  Wire.write(number);
}

void sendDataR() {
  Wire.write(nbr);
}


