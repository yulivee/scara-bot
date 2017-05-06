#include <Arduino.h>
#include <move.h>
#include <pid.h>
#include <pins.h>
int monitor = 0;
int clicks = 20;

struct pins motor_pins = { 5, 6, 4, 2, 3 };
volatile struct counts motor_cnt = { 0, 0 };

volatile int flag = 0;                                                                                                         
void doCount0() {                                                                                                            
   cli();                                                                                                                               
   flag = digitalRead(motor_pins.cnt1);                                                                                     
                                                                                                                                          
   if ( flag == 1 ) {                                                                                                       
      motor_cnt.cnt0++;                                                                                                    
   } else {                                                                                                                 
      motor_cnt.cnt0--;                                                                                                    
   } 
   sei();
} 

void setup()
{
    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(motor_pins.cnt0), doCount0, RISING);
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
	Serial.println(motor_cnt.cnt0);
         monitor = Serial.read();

        if( monitor == 49 ) { // aus
            stop_motors(&motor_pins);
        }
        if ( monitor == 121 ) {
	    Serial.print("Target: ");
	    Serial.println(motor_cnt.cnt0+clicks);
	    Serial.print("Current: ");
	    Serial.println(motor_cnt.cnt0);
            move(&clicks, &motor_pins, &motor_cnt, RIGHT);
	    Serial.print("After movement: ");
	    Serial.println(motor_cnt.cnt0);
	    
        }
        if ( monitor == 120 ) {
	    	
	    Serial.print("Target: ");
	    Serial.println(motor_cnt.cnt0-clicks);
	    Serial.print("Current: ");
	    Serial.println(motor_cnt.cnt0);
            move(&clicks, &motor_pins, &motor_cnt, LEFT);
	    Serial.print("After movement: ");
	    Serial.println(motor_cnt.cnt0);
	    
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
    delay(100);
}
