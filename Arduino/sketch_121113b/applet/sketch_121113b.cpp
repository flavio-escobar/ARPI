#include < Servo.h >

#include "WProgram.h"
void setup();
void loop();
Servo servo1; //defenir os nomes dos servos

void setup()
{
servo1.attach(9); // ligar um servo1 ao pino 9

Serial.begin(9600);

servo1.write(0); // servos iniciao na posi\u00e7\u00e3o 0
}
void loop()
{
motor.write(160);
delay(1000);
motor.write(16);
delay(1000);
}

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

