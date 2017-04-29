#include <Arduino.h>
#include <pins.h>
int monitor = 0;

int tempo = 30;
int clicks = 20;

void doCount0() {
    uaj.cnt0++;
}
void doCount1() {
    uaj.cnt1++;
}

void setup()
{
    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(uaj.cnt0), doCount0, CHANGE);
    attachInterrupt(digitalPinToInterrupt(uaj.cnt1), doCount1, CHANGE);
    pinMode(uaj.cnt0, INPUT);
    pinMode(uaj.cnt1, INPUT);
    pinMode(uaj.enable, OUTPUT);
    pinMode(uaj.left, OUTPUT);
    pinMode(uaj.right, OUTPUT);

    digitalWrite(uaj.left, 0);
    digitalWrite(uaj.right, 0);
}

void stop_motors (struct pins *input) {
    digitalWrite(input->left, 0 );
    digitalWrite(input->right, 0 );
    digitalWrite(input->enable, 0);
}

void move ( struct pins *input, int direction ) {

    Serial.println(input->cnt0);
    int target_count = input->cnt0;
    int start_value = input->cnt0;
    target_count += clicks;
    Serial.print(">> ");
    Serial.print(input->cnt0);
    Serial.println(" <<");
    digitalWrite(input->enable, 1);

    while ( target_count > input->cnt0 ) {
        if ( direction == RIGHT ) {
            digitalWrite(input->right, 1);
        } else {
            digitalWrite(input->left, 1);
        }
    }

    stop_motors(input);

    Serial.print(">> ");
    Serial.print(input->cnt0);
    Serial.print(" >> ");
    Serial.print(input->cnt0 - start_value);
    Serial.println(" <<");


}

void loop()
{

    if (Serial.available() > 0) {
        monitor = Serial.read();

        if( monitor == 49 ) { // aus
            stop_motors(&uaj);
        }
        if ( monitor == 121 ) {
            move(&uaj,RIGHT);
        }
        if ( monitor == 120 ) {
            move(&uaj,LEFT);
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
    delay(1000);
}
