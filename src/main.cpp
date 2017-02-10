#include <Arduino.h>
#include "pins.h"

int monitor = 0;

int tempo = 100;

void setup()
{
    Serial.begin(9600);
    pinMode(SHOULDER_LEFT, OUTPUT);
    pinMode(SHOULDER_RIGHT, OUTPUT);
    pinMode(UAE_LEFT, OUTPUT);
    pinMode(UAE_RIGHT, OUTPUT);
    pinMode(UAJ_LEFT, OUTPUT);
    pinMode(UAJ_RIGHT, OUTPUT);
    pinMode(SKG_LEFT, OUTPUT);
    pinMode(SKG_RIGHT, OUTPUT);
    pinMode(SKF_LEFT, OUTPUT);
    pinMode(SKF_RIGHT, OUTPUT);
    pinMode(SKE_LEFT, OUTPUT);
    pinMode(SKE_RIGHT, OUTPUT);

    analogWrite(SHOULDER_LEFT, 0);
    analogWrite(SHOULDER_RIGHT, 0);
    analogWrite(UAE_LEFT, 0);
    analogWrite(UAE_RIGHT, 0);
    analogWrite(UAJ_LEFT, 0);
    analogWrite(UAJ_RIGHT, 0);
    analogWrite(SKG_LEFT, 0);
    analogWrite(SKG_RIGHT, 0);
    analogWrite(SKF_LEFT, 0);
    analogWrite(SKF_RIGHT, 0);
    analogWrite(SKE_LEFT, 0);
    analogWrite(SKE_RIGHT, 0);
}
void stop_motors () {
    analogWrite(SHOULDER_LEFT, 0);
    analogWrite(SHOULDER_RIGHT, 0);
    analogWrite(UAE_LEFT, 0);
    analogWrite(UAE_RIGHT, 0);
    analogWrite(UAJ_LEFT, 0);
    analogWrite(UAJ_RIGHT, 0);
    analogWrite(SKG_LEFT, 0);
    analogWrite(SKG_RIGHT, 0);
    analogWrite(SKF_LEFT, 0);
    analogWrite(SKF_RIGHT, 0);
    analogWrite(SKE_LEFT, 0);
    analogWrite(SKE_RIGHT, 0);
}

void move_shoulder_right () {
    analogWrite(SHOULDER_LEFT, tempo);
    analogWrite(SHOULDER_RIGHT, 0 );
    delay(1000);
    stop_motors();
}

void move_shoulder_left () {
    analogWrite(SHOULDER_LEFT, 0);
    analogWrite(SHOULDER_RIGHT, tempo );
    delay(1000);
    stop_motors();
}

void move_uae_right () {
    analogWrite(UAE_LEFT,tempo);
    analogWrite(UAE_RIGHT, 0);
    delay(1000);
    stop_motors();
}

void move_uae_left () {
    analogWrite(UAE_LEFT, 0);
    analogWrite(UAE_RIGHT, tempo);
    delay(1000);
    stop_motors();
}

void move_uaj_right () {
    analogWrite(UAJ_LEFT,tempo);
    analogWrite(UAJ_RIGHT, 0);
    delay(1000);
    stop_motors();
}

void move_uaj_left () {
    analogWrite(UAJ_LEFT, 0);
    analogWrite(UAJ_RIGHT, tempo);
    delay(1000);
    stop_motors();
}

void move_ske_right () {
    analogWrite(SKE_LEFT,tempo);
    analogWrite(SKE_RIGHT, 0);
    delay(1000);
    stop_motors();
}

void move_ske_left () {
    analogWrite(SKE_LEFT, 0);
    analogWrite(SKE_RIGHT, tempo);
    delay(1000);
    stop_motors();
}

void move_skg_right () {
    analogWrite(SKG_LEFT,tempo);
    analogWrite(SKG_RIGHT, 0);
    delay(1000);
    stop_motors();
}

void move_skg_left () {
    analogWrite(SKG_LEFT, 0);
    analogWrite(SKG_RIGHT, tempo);
    delay(1000);
    stop_motors();
}

void move_skf_right () {
    analogWrite(SKF_LEFT,tempo);
    analogWrite(SKF_RIGHT, 0);
    delay(1000);
    stop_motors();
}

void move_skf_left () {
    analogWrite(SKF_LEFT, 0);
    analogWrite(SKF_RIGHT, tempo);
    delay(1000);
    stop_motors();
}

void loop()
{

    if (Serial.available() > 0) {
        monitor = Serial.read();

        if( monitor == 49 ) { // aus
            stop_motors();
        }

        if( monitor == 113 ) {
            move_shoulder_right();
            monitor = 49;
        }

        if( monitor == 119 ) {
            move_shoulder_left();
            monitor = 49;
        }
        if( monitor == 97 ) {
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
            move_uaj_left();
            monitor = 49;

        }
	if ( monitor == 101 ) {
	    move_ske_right();
            monitor = 49;
	}
	if ( monitor == 114 ) {
	    move_ske_left();
            monitor = 49;
	}
	if ( monitor == 100 ) {
	    move_skf_right();
            monitor = 49;
	}
	if ( monitor == 102 ) {
	    move_skf_left();
            monitor = 49;
	}
	if ( monitor == 99 ) {
	    move_skg_right();
            monitor = 49;
	}
	if ( monitor == 118 ) {
	    move_skg_left();
            monitor = 49;
	}
        Serial.println( monitor );
    }
    delay(100);
}

