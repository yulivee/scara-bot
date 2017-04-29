#include <Arduino.h>
#include <pins.h>
volatile int cnt0 = 0;
volatile int cnt1 = 0;
int monitor = 0;

int tempo = 30;
int clicks = 2;

void doCount0() {
    cnt0++;
}
void doCount1() {
    cnt1++;
}

void setup()
{
    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(uaj.cnt0), doCount0, CHANGE);
    attachInterrupt(digitalPinToInterrupt(uaj.cnt1), doCount1, CHANGE);
    pinMode(uaj.cnt0, INPUT);
    pinMode(uaj.cnt1, INPUT);
    pinMode(uaj.left, OUTPUT);
    pinMode(uaj.right, OUTPUT);

    digitalWrite(uaj.left, 0);
    digitalWrite(uaj.right, 0);
}

void stop_motors () {
    digitalWrite(uaj.left, 0);
    digitalWrite(uaj.right, 0);
}

void move ( struct pins *input, int direction ) {

    Serial.println(cnt0);
    int current_enc = cnt0;
    int start_value = cnt0;
    current_enc += clicks;
    Serial.print(">> ");
    Serial.print(cnt0);
    Serial.println(" <<");

    while ( current_enc > cnt0 ) {
        if ( direction == RIGHT ) {
            digitalWrite(input->right, 1);
        } else {
            digitalWrite(input->left, 1);
        }
    }
    digitalWrite(input->left, 0 );
    digitalWrite(input->right, 0 );
    digitalWrite(input->enable, 0);

    Serial.print(">> ");
    Serial.print(cnt0);
    Serial.print(" >> ");
    Serial.print(cnt0 - start_value);
    Serial.println(" <<");

    stop_motors();
}

void loop()
{

    if (Serial.available() > 0) {
        monitor = Serial.read();

        if( monitor == 49 ) { // aus
            stop_motors();
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

