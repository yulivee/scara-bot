#include <Arduino.h>
#include <avr/delay.h>
#include <pid.h>
#include <pins.h>

//MADA
int joint = 3; //diese Zahl anpassen je nachdem welches Gelenk angesteuert werden soll von dem Chip
float mada[3][7]={
  {15,15,15,15,15,15,15}, //P-Anteile
  {0.6,0.6,0.6,0.6,0.6,0.6,0.6}, //I-Anteile
  {12,12,12,12,12,12,12} //D-Anteile
};

// VARIABLEN
int monitor = 0;
int clicks = 300;
struct pins motor_pins = { 10, 11, 4, 2, 3 };
volatile struct counts motor_cnt = { 0, 0 };
volatile int flag_0 = 0;
volatile int flag_1 = 0;
volatile int positionDelta, positionSpeed, positionLastDelta, positionDiff, positionInt = 0;
volatile int target_position = 0;
volatile int current_position = 0;
float MOVEP, MOVEI, MOVED; //PID Werte, Werden aus den Mada zugewiesen
int MOVEIMAX, MOVEMAX;

//FUNKTIONEN
void count_encoder() {//Interruptfunktion zum Auswerten des Encoders
    flag_0 = digitalRead(motor_pins.cnt0);
    flag_1 = digitalRead(motor_pins.cnt1);

    //Durch Vergleich der beiden Lichtschranken kann die Drehrichtung ermittelt werden.
    if ( flag_0 == flag_1 ) {
        motor_cnt.cnt0++;
    } else {
        motor_cnt.cnt0--;
    }
}

void setMADA(){
  MOVEP = mada[0][joint-1];
  MOVEI = mada[1][joint-1];
  MOVED = mada[2][joint-1];
  MOVEIMAX = 100;
  MOVEMAX = 255;
}

//INITIALISIERUNG
void setup()
{
    //Starten der Seriellen Schnittstelle
    Serial.begin(115200);
    Serial.println("Chili-Bot ready");
    //Deklarienen des Interrupts für den Motorencoder
    attachInterrupt(digitalPinToInterrupt(motor_pins.cnt0), count_encoder(), CHANGE);
    //Einlesen der MADA
    setMADA();
    //Starten des Timers ???
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
