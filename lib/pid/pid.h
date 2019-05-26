// --------------------------------------
// Name: pid.h
// Project: scara-bot
// Description: header file for pid.c, definition of constants
//---------------------------------------

#ifndef PID_H_INCLUDED
#define PID_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

//controller specific PID values
//TODO read these values from Ros parameter server
#define MOVEP 15 //P Motorspannung pro Schritt [V/Klicks] - - Genauigkeit. Erzeugt Zittern und schießt übers Ziel raus
#define MOVEI 0.6 //I Erhöht Motorspannung bei kleinen Fehlern - lässt die letzten bisschen Fehler verschwinden
#define MOVED 12 //D Rechtzeitig bremsen - dämpft Zittern und rauschießen

//universal PID values
#define MOVEMAX 255
#define MOVEIMAX 100
#define GOVERNOR_FREQ 100

// Max cap for delta value to avoid variable overflow
#define MOVEDELTAMAX 255

//function to check if a value is in bounds
#define BOUNDS(var, max) if ((var)>(max)) (var)=(max); if ( (var)<-(max)) (var)=-(max);

//function declarations:
void timer_init();

#ifdef __cplusplus
}
#endif

#endif
