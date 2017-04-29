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
    attachInterrupt(digitalPinToInterrupt(19), doCount0, CHANGE); // Interrupt 4 liegt auf Pin 19
    attachInterrupt(5, doCount1, CHANGE); // Interrupt 5 liegt auf Pin 18
    pinMode(19, INPUT);
    pinMode(shoulder.left, OUTPUT);
    pinMode(shoulder.right, OUTPUT);
    pinMode(shoulder.enable, OUTPUT);
    pinMode(uae.left, OUTPUT);
    pinMode(uae.right, OUTPUT);
    pinMode(uaj.left, OUTPUT);
    pinMode(uaj.right, OUTPUT);
    pinMode(skg.left, OUTPUT);
    pinMode(skg.right, OUTPUT);
    pinMode(skf.left, OUTPUT);
    pinMode(skf.right, OUTPUT);
    pinMode(ske.left, OUTPUT);
    pinMode(ske.right, OUTPUT);
    pinMode(zaxis.left, OUTPUT);
    pinMode(zaxis.right, OUTPUT);

    digitalWrite(shoulder.left, 0);
    digitalWrite(shoulder.right, 0);
    digitalWrite(uae.left, 0);
    digitalWrite(uae.right, 0);
    digitalWrite(uaj.left, 0);
    digitalWrite(uaj.right, 0);
    digitalWrite(skg.left, 0);
    digitalWrite(skg.right, 0);
    digitalWrite(skf.left, 0);
    digitalWrite(skf.right, 0);
    digitalWrite(ske.left, 0);
    digitalWrite(ske.right, 0);
    digitalWrite(zaxis.left, 0);
    digitalWrite(zaxis.right, 0);
}

void stop_motors () {
    digitalWrite(shoulder.left, 0);
    digitalWrite(shoulder.right, 0);
    digitalWrite(uae.left, 0);
    digitalWrite(uae.right, 0);
    digitalWrite(uaj.left, 0);
    digitalWrite(uaj.right, 0);
    digitalWrite(skg.left, 0);
    digitalWrite(skg.right, 0);
    digitalWrite(skf.left, 0);
    digitalWrite(skf.right, 0);
    digitalWrite(ske.left, 0);
    digitalWrite(ske.right, 0);
    digitalWrite(zaxis.left, 0);
    digitalWrite(zaxis.right, 0);
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
        //Serial.print("-- ");
        //Serial.print(cnt0);
        //Serial.println(" --");
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

        if( monitor == 113 ) {
            move(&shoulder,RIGHT);
        }

        if( monitor == 119 ) {
            move(&shoulder,LEFT);
        }
        if( monitor == 97 ) {
            move(&uae,LEFT);
        }
        if ( monitor == 115 ) {
            move(&uae,RIGHT);
        }
        if ( monitor == 121 ) {
            move(&uaj,RIGHT);
        }
        if ( monitor == 120 ) {
            move(&uaj,LEFT);
        }
        if ( monitor == 101 ) {
            move(&ske,RIGHT);
        }
        if ( monitor == 114 ) {
            move(&ske,LEFT);
        }
        if ( monitor == 100 ) {
            move(&skf,RIGHT);
        }
        if ( monitor == 102 ) {
            move(&skf,LEFT);
        }
        if ( monitor == 99 ) {
            move(&skg,RIGHT);
        }
        if ( monitor == 118 ) {
            move(&skg,LEFT);
        }
        if ( monitor == 116 ) {
            move(&zaxis,LEFT);
        }
        if ( monitor == 122 ) {
            move(&zaxis,RIGHT);
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

