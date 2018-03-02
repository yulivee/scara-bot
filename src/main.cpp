#include <Arduino.h>
#include <avr/delay.h>
#include <pid.h>
#include <pins.h>

// VARIABLEN
int monitor = 0;
int clicks = 300;

struct pins motor_pins = { 10, 11, 4, 2, 3 };
volatile struct counts motor_cnt = { 0, 0 };
volatile int positionDelta, positionSpeed, positionLastDelta, positionDiff, positionInt = 0;
volatile int target_position = 0;
volatile int current_position = 0;
volatile int flag_0 = 0;
volatile int flag_1 = 0;

//FUNKTIONEN
//Interruptfunktion zum Auswerten des Encoders
void count_encoder() {
    flag_0 = digitalRead(motor_pins.cnt0);
    flag_1 = digitalRead(motor_pins.cnt1);

    //Durch Vergleich der beiden Lichtschranken kann die Drehrichtung ermittelt werden.
    if ( flag_0 == flag_1 ) {
        motor_cnt.cnt0++;
    } else {
        motor_cnt.cnt0--;
    }
}

//INITIALISIERUNG
void setup()
{
    //Starten der Seriellen Schnittstelle
    Serial.begin(115200);
    Serial.println("Chili-Bot ready");
    //Deklarienen des Interrupts für den Motorencoder
    attachInterrupt(digitalPinToInterrupt(motor_pins.cnt0), count_encoder(), CHANGE);
    //Starten des Timers für Delay-Funktionen
    timer_init();
    //Initialisren der Pins
    pinMode(motor_pins.cnt0, INPUT);
    pinMode(motor_pins.cnt1, INPUT);
    pinMode(motor_pins.enable, OUTPUT);
    pinMode(motor_pins.left, OUTPUT);
    pinMode(motor_pins.right, OUTPUT);
    //Ansteuern der Motorcontroller
    digitalWrite(motor_pins.left, 0);
    digitalWrite(motor_pins.right, 0);
    digitalWrite(motor_pins.enable, 1);
}

//HAUPTPROGRAMMSCHLEIFE
void loop()
{
    //Auslesen eines Tastendrucks und ausgeben
    monitor = Serial.read();
    //Ausgeben des Sollwertes
    Serial.print("Target: ");
    Serial.println(target_position);
    Serial.print("Current: ");
    Serial.println(motor_cnt.cnt0);
    Serial.print("Speed: ");
    Serial.println(positionSpeed);
    Serial.println("-------------");

    //Auswerten des Tasteneigabe
    if( monitor == 49 ) { //1 - Motor aus
        digitalWrite(motor_pins.left, 0);
        digitalWrite(motor_pins.right, 0);
    }
    if ( monitor == 121 ) { //y - Sollwert um Schrittgröße erhöhen
        target_position += clicks;
    }
    if ( monitor == 120 ) { //x - Sollwert um Schrittgröße verringern
        target_position -= clicks;
    }
    if ( monitor == 107 ) { //k - Schrittgröße verringern
        clicks-=5;
        Serial.print ("Anzahl Schritte: ");
        Serial.println( clicks );
    }
    if ( monitor == 108 ) { //l - Schrittgröße erhöhen
        clicks+=5;
        Serial.print ("Anzahl Schritte: ");
        Serial.println( clicks );
    }

    _delay_ms(500);
}
