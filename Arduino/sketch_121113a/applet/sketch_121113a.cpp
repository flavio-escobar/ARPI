#include <Servo.h>

#include "WProgram.h"
void setup();
void loop();
Servo servo1;
Servo servo2;

char buffer[4];
int received;

void setup(){
  
  //Inicia a porta serial
  Serial.begin(9600);
  
  //Atribui as entradas do Arduino
  //as variaveis
  servo1.attach(8);
  servo2.attach(9);
  
  //Movimentamos ambos os motores para 90 graus
  servo1.write(90);
  servo2.write(90);
  
  received = 0;
}

void loop(){
  
  //Verifica se possui alguma entrada de dados
  //na entrada Serial
  if(Serial.available()){
    
    char c = Serial.read();
    
    // Se o "c" for igual ao caracter "A" (97 na tabela ASCII)
    if(c == 97){
      Serial.println(buffer);
      int numero = atoi(buffer);
      servo1.write(numero);
      received = 0; 
    }
    
    // Se o "c" for igual ao caracter "B" (98 na tabela ASCII)
    if(c == 98){
      Serial.println(buffer);
      int numero = atoi(buffer);
      if(numero >= 60){
        servo2.write(numero);
        received = 0; 
      }
      received = 0; 
    }
    
    if(c != 97 && c != 98){
      buffer[received++] = c;
    }
    
    Serial.flush();
  }

}
int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

