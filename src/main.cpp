#include <Arduino.h>
int links1 = 6;
int rechts1 = 5;
int enable1 = 4;
int links2 = 9;
int rechts2 = 10;
int enable2 = 7;
int rechts3 = ;
int links3 = ;
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
void stop_motors () {
    analogWrite(links1, 0);
    analogWrite(rechts1, 0);
    analogWrite(links2, 0);
    analogWrite(rechts2, 0);
}
void move_shoulder_right () {
    analogWrite(links1, tempo);
    analogWrite(rechts1, 0 );
    delay(1000);
    stop_motors();
}
void move_shoulder_left () {
    analogWrite(links1, 0);
    analogWrite(rechts1, tempo );
    delay(1000);
    stop_motors();
}
void move_uae_right () {
    analogWrite(links2,tempo);
    analogWrite(rechts2, 0);
    delay(1000);
    stop_motors();
}
void move_uae_left () {
    analogWrite(links2, 0);
    analogWrite(rechts2, tempo);
    delay(1000);
    stop_motors();
}

void move_uaj_right () {
    analogWrite(links3,tempo);
    analogWrite(rechts3, 0);
    delay(1000);
    stop_motors();
}
void move_uaj_left () {
    analogWrite(links3, 0);
    analogWrite(rechts3, tempo);
    delay(1000);
    stop_motors();
}
void loop()
{

    if (Serial.available() > 0) {
        monitor = Serial.read();

        if(monitor == 49) { // aus
            stop_motors();
            //analogWrite(links1, 0);
            //analogWrite(rechts1, 0);
            //analogWrite(links2, 0);
            //analogWrite(rechts2, 0);
        }

        if(monitor == 113) {
//   analogWrite(links1, tempo);
//   analogWrite(rechts1, 0);
            move_shoulder_right();
            monitor = 49;
        }

        if(monitor == 119) {
//   analogWrite(links1, 0);
//   analogWrite(rechts1, tempo);
            move_shoulder_left();
            monitor = 49;
        }
        if( monitor == 97) {
            move_uae_left();
            monitor = 49;
        }
        if ( monitor == 115 ) {
            move_uae_right();
            monitor = 49;

        }
        if ( monitor == 121 ) {
            move_uaj_right();
            monitor = 49;

        }
        if ( monitor == 120 ) {
            move_uaj_right();
            monitor = 49;

        }
        Serial.println(monitor);
    }
    delay(100);
}

