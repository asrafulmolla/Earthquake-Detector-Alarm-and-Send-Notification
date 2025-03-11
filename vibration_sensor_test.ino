// Created  by Fair Electro.......

int Sensor_pin=7;
int Buzzer_pin=11;
void setup() {
  pinMode(Sensor_pin,INPUT);
  pinMode(Buzzer_pin,OUTPUT);
}

void loop() {
  int data;
  data=digitalRead(Sensor_pin);
  if(data==1)
  {
    digitalWrite(Buzzer_pin,HIGH);
    delay(200);
    digitalWrite(Buzzer_pin,LOW);
    delay(200);
        digitalWrite(Buzzer_pin,HIGH);
    delay(200);
    digitalWrite(Buzzer_pin,LOW);
    delay(200);
       digitalWrite(Buzzer_pin,HIGH);
    delay(200);
    digitalWrite(Buzzer_pin,LOW);
    delay(200);
   }
   else
   digitalWrite(Buzzer_pin,LOW);
}
