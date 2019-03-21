// --------------------------------------
// Name: pid.c
// Project: scara-bot
// Description: implementation of PID controller
//---------------------------------------

#include <pins.h>
#include <pid.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

void timer_init() {
    //Set registers to start timer interrupt, with freqency 156.25 hertz ~ 6ms
    TCCR0A=(1<<WGM01);
    TCCR0B=(5<<CS00);
    OCR0A=F_CPU/1024/GOVERNOR_FREQ;
    TIMSK0=(1<<OCIE0A);
}

ISR(TIMER0_COMPA_vect) {

    //set first pin of regiser B to output (Aduino Pin D8/AVR Pin PB0)
    //DDRB|=0x20;
    //set output HIGH
    //PORTB|=0x20;

    //calculate values for PID controller
    positionDelta = target_position - motor_cnt;
    positionDiff = positionDelta - positionLastDelta;
    positionLastDelta = positionDelta;
    positionInt += positionDelta;
    BOUNDS ( positionInt , MOVEIMAX );

    if ( target_position == motor_cnt ) {
        positionSpeed = 0;
    } else {
        //caluclate speed with PID control
      positionSpeed = positionDelta * MOVEP + positionInt * MOVEI + positionDiff * MOVED;
      BOUNDS ( positionSpeed , MOVEMAX );
    }

    //set Motors with speed value
    if ( positionSpeed >= 0 ) {
        analogWrite(motor_pins.left, 0);
        analogWrite(motor_pins.right, positionSpeed);

    } else {
        analogWrite(motor_pins.right, 0);
        analogWrite(motor_pins.left, 0 - positionSpeed);
    }

    //AND with 011111 on PORTB, set firt pin LOW
    //PORTB&=~0x20;

}

#ifdef __cplusplus
}
#endif
