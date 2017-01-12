#include <Arduino.h>
int links1 = 6;
int rechts1 = 5;
int enable1 = 4;
int links2 = 9;
int rechts2 = 10;
int enable2 = 7;
int monitor = 0;

int tempo = 100;

void setup()
{
  Serial.begin(9600);
  pinMode(enable1, OUTPUT);
  digitalWrite(enable1, HIGH);
  pinMode(enable2, OUTPUT);
  digitalWrite(enable2, HIGH);
  pinMode(links1, OUTPUT);
  pinMode(rechts1, OUTPUT);
  pinMode(links2, OUTPUT);
  pinMode(rechts2, OUTPUT);


  analogWrite(links1, 0);
  analogWrite(rechts1, 0);
  analogWrite(links2, 0);
  analogWrite(rechts2, 0);
}

void loop()
{

if (Serial.available() > 0) {
monitor = Serial.read();

if(monitor == 49){ // aus
   analogWrite(links1, 0);
   analogWrite(rechts1, 0);
   analogWrite(links2, 0);
   analogWrite(rechts2, 0);
    }

if(monitor == 50){
   analogWrite(links1, tempo);
   analogWrite(rechts1, 0);
    }

 if(monitor == 51){
   analogWrite(links1, 0);
   analogWrite(rechts1, tempo);
    }
if( monitor == 52){
  analogWrite(links2,tempo);
  analogWrite(rechts2, 0);
}
if ( monitor == 53 ) {
  analogWrite(links2, 0);
  analogWrite(rechts2, tempo);

}
 Serial.println(monitor);
  }
delay(100);
}
