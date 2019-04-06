// --------------------------------------
// Name: pins.h
// Project: scara-bot
// Description: definition of motor pin variables
//---------------------------------------

#ifndef PINS_H_INCLUDED
#define PINS_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

//define structures and variables for interrupts
struct pins { int left; int right; int enable; int cnt0; int cnt1; };
extern struct pins motor_pins;
extern volatile int motor_cnt;
extern int target_position;
extern volatile int positionSpeed;
extern volatile int positionDelta, positionLastDelta, positionDiff, positionInt;

#ifdef __cplusplus
}
#endif

#endif
