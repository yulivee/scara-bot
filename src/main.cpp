#include <Arduino.h>
#include <move.h>
#include <pid.h>
#include <pins.h>
#include <interrupt.h>
int monitor = 0;

void setup()
{
    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(motor_pins.cnt0), doCount0, HIGH);
    attachInterrupt(digitalPinToInterrupt(motor_pins.cnt1), doCount1, CHANGE);
    pinMode(motor_pins.cnt0, INPUT);
    pinMode(motor_pins.cnt1, INPUT);
    pinMode(motor_pins.enable, OUTPUT);
    pinMode(motor_pins.left, OUTPUT);
    pinMode(motor_pins.right, OUTPUT);

    digitalWrite(motor_pins.left, 0);
    digitalWrite(motor_pins.right, 0);
}

void loop()
{

    if (Serial.available() > 0) {
        monitor = Serial.read();

        if( monitor == 49 ) { // aus
            stop_motors(&motor);
        }
        if ( monitor == 121 ) {
            move(&motor,RIGHT);
        }
        if ( monitor == 120 ) {
            move(&motor,LEFT);
        }
        if ( monitor == 111 ) {
            tempo-=5;
            Serial.print ("Neues Tempo: ");
            Serial.println( tempo );
        }
        if ( monitor == 112 ) {
            tempo+=5;
            Serial.print ("Neues Tempo: ");
            Serial.println( tempo );
        }
        if ( monitor == 107 ) {
            clicks-=5;
            Serial.print ("Anzahl Schritte: ");
            Serial.println( clicks );
        }
        if ( monitor == 108 ) {
            clicks+=5;
            Serial.print ("Anzahl Schritte: ");
            Serial.println( clicks );
        }
        Serial.println( monitor );
    }
    delay(500);
}
