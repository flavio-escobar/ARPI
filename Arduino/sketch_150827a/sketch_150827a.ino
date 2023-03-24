unsigned int i = 0;

void setup(){
Serial.begin(9600);
delay(200);

i = 66000;
Serial.print(i);
}

void loop(){
delay(100);
}

