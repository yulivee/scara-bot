#include <Arduino.h>
#include <avr/delay.h>
#include <pid.h>
#include <pins.h>
int monitor = 0;
int clicks = 300;

struct pins motor_pins = { 10, 11, 4, 2, 3 };
volatile struct counts motor_cnt = { 0, 0 };
volatile int positionDelta, positionSpeed, positionLastDelta, positionDiff, positionInt = 0;
volatile int target_position = 0;
volatile int current_position = 0;
volatile int flag = 0;                                                                                                         

void doCount0() {                                                                                                            
   flag = digitalRead(motor_pins.cnt1);                                                                                     
                                                                                                                                          
   if ( flag == 1 ) {                                                                                                       
      motor_cnt.cnt0++;                                                                                                    
   } else {                                                                                                                 
      motor_cnt.cnt0--;                                                                                                    
   } 
} 

void setup()
{
    Serial.begin(115200);
    Serial.println("Scara-Bot ready"); 
    attachInterrupt(digitalPinToInterrupt(motor_pins.cnt0), doCount0, RISING);
    timer_init();
    pinMode(motor_pins.cnt0, INPUT);
    pinMode(motor_pins.cnt1, INPUT);
    pinMode(motor_pins.enable, OUTPUT);
    pinMode(motor_pins.left, OUTPUT);
    pinMode(motor_pins.right, OUTPUT);

    digitalWrite(motor_pins.left, 0);
    digitalWrite(motor_pins.right, 0);
    digitalWrite(motor_pins.enable, 1);
}

void loop()
{

         monitor = Serial.read();
	 Serial.print("Target: ");
	 Serial.println(target_position);
	 Serial.print("Current: ");
	 Serial.println(motor_cnt.cnt0);
	 Serial.print("Speed: ");
	 Serial.println(positionSpeed);
	 Serial.println("-------------");

        if( monitor == 49 ) { // aus
	    digitalWrite(motor_pins.left, 0);
	    digitalWrite(motor_pins.right, 0);
        }
        if ( monitor == 121 ) {
	    target_position += clicks;
        }
        if ( monitor == 120 ) {
	    target_position -= clicks;
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
        //Serial.println( monitor );

    _delay_ms(500);
}
