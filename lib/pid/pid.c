#include <pins.h>
#include <pid.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#ifdef __cplusplus
extern "C" {
#endif

void timer_init( int *governor_freq ) {
    //Set registers to start timer interrupt, with freqency 156.25 hertz ~ 6ms
    TCCR0A=(1<<WGM01);
    TCCR0B=(5<<CS00);
    OCR0A=F_CPU/1024/(*govenor_freq);
    TIMSK0=(1<<OCIE0A);
}

ISR(TIMER0_COMPA_vect) {

    //set first pin of regiser B to output (Aduino Pin D8/AVR Pin PB0)
    DDRB|=0x20;
    //set output HIGH
    PORTB|=0x20;

    //calculate values for PID controller
    pid_position.delta = target_position - motor_cnt;
    pid_position.diff = pid_position.delta - pid_position.last_delta;
    pid_position.last_delta = pid_position.delta;
    pid_position.integral = pid_position.delta;
    BOUNDS ( pid_position.integral , pid_parameters.int_max );

    if ( target_position == motor_cnt ) {
        pid_position.speed = 0;
    } else {
        //caluclate speed with PID control
        pid_position.speed = pid_position.delta * pid_parameters.p + pid_position.integral * pid_parameters.i + pid_position.diff * pid_parameters.d;
    }
    BOUNDS ( pid_position.speed , pid_parameters.max_speed );

    //set Motors with speed value
    if ( pid_position.speed >= 0 ) {
        analogWrite(motor_pins.left, 0);
        analogWrite(motor_pins.right, pid_position.speed);

    } else {
        analogWrite(motor_pins.right, 0);
        analogWrite(motor_pins.left, 0 - pid_position.speed);
    }

    //AND with 011111 on PORTB, set firt pin LOW
    PORTB&=~0x20;

}

#ifdef __cplusplus
}
#endif
