#include <Wire.h>
#include "EEPROMAnything.h"
#include <EEPROM.h>

int number = 0;
int state = 0;
int address = 0;
int backdoor = 0;
int op = 0;
int estadoBotao = 0;
int pwm = 8;

struct config_t{
  int writed;
  int address;
  int number;
  int state;
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
  Serial.print("Estado guardado:");
  Serial.println(configuration.state);
  
  if(configuration.writed == 1){
    address = configuration.address;
    number = configuration.number;
    state = configuration.state;
  Serial.println("Dados Recuperados!!");
  }else{
    address = 0;
    state = 0;
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
  configuration.state = state;
  
  EEPROM_writeAnything(0, configuration);
  
  Serial.print("Writed: ");
  Serial.println(configuration.writed);
  Serial.print("Endereco gravado: ");
  Serial.println(configuration.address);
  Serial.print("Numero: ");
  Serial.println(configuration.number);
  Serial.print("Estado(0-operacao|1-informacao): ");
  Serial.println(configuration.state);
  
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
  delay(100);
}

void interrupcao(){
  estadoBotao = digitalRead(40);
  if(estadoBotao == HIGH){
    analogWrite(pwm, 0);
    digitalWrite(13, HIGH);
    backdoor = 5;
  }else{
    digitalWrite(13, LOW);
  }
}

//MODULOS PROGRAMAVEIS\\

/*
  -Modulo I-
Reenderecamento
*/
void receiveAddr(int data) {

  address = data;
  state = 0;  
  Serial.print("Endereco Recebido: ");
  Serial.println(address);
    
  saveValues();
  asm volatile ("  jmp 0");
}

/*
-Modulo II-
Comunicação
*/
void receiveInf(int data) {
  
  if(data != 255){
    number = data;
  }
  Serial.print("\n*receiveInf*Dado Recebido: ");
  Serial.println(data);
}
/*
-Modulo III-
Retorno da interrupcao
*/

void receiveRet(int data){

  if(data == 5){
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    Serial.print("Inserir pedido de interrupção");
  }
  if(data == 4){
    Serial.print("Sem pedidos de Interrupcao");
  }
}

int gerenciadorOP(){
  
  int aux = 0;
  
  if(op == 1){
      Serial.print("\n-----------------REENDERECANDO------------------\n");
      Wire.onRequest(sendDataR);
      aux = 1;
    }
    if(op == 2){
      Wire.onRequest(sendDataN);
      aux = 0;
    }
    if(op == 3){
      Wire.onRequest(sendDataR);
      aux = 1;
    }
    if(op == 4){
      Serial.print("Backdoor: ");
      Serial.print(backdoor);
      if (backdoor == 5){
        Wire.onRequest(sendDataS);
        backdoor = 0;
      }else{
        Wire.onRequest(sendDataR);
      }
      aux = 1;
      state = 0;
    }
    
    return(aux);
}

int gerenciadorINF(int data){
  
  if(op == 1){
      receiveAddr(data);
      data = 0;
    }
    if(op == 2){
       data = 0;
    }
    if(op == 3){
      receiveInf(data);
      data = 0;
    }
    if(op == 4){
      receiveRet(data);
      data = 0;
    }
  
  return(data);
  
}


// callback for received data
void receiveData(int byteCount) {
  
  int data = 0;
  int aux = 0;
  
  while(Wire.available()){
    aux = Wire.read();
    Serial.print("Dado Recebido: ");
    Serial.println(aux);

    if(state == 0){
      op = aux;
      state = gerenciadorOP();
    }else if(state == 1){
      data = aux;
      state = gerenciadorINF(data);
    }else{
      Serial.print("Erro, Caminho inesperado!!");
      state = 0;
    }
    saveValues();
  }
}

void sendDataR(){
  Wire.write(op);
}
void sendDataN(){
  Wire.write(number);
}
void sendDataS(){
  Wire.write(backdoor);
}

