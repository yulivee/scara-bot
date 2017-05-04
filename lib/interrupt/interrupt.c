#include <Arduino.h>
#include <interrupt.h>
#include <pins.h>

#ifdef __cplusplus
extern "C" {
#endif

void doCount0() {

    flag = digitalRead(motor_pins.cnt1);

    if ( flag == 1 ) {
	    motor_cnt.cnt0++;
    } else {
	    motor_cnt.cnt0--;
    }
}

void doCount1() {

    motor_cnt.cnt1++;
}

#ifdef __cplusplus
}
#endif
