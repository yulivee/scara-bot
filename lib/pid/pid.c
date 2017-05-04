#include <Arduino.h>
#include <pins.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MOVEP 0.3
#define MOVEI 0.1 //0.5
#define MOVED 0.6
#define MOVEMAX 255 //25
#define MOVEIMAX 200    
#define BOUNDS(var, max) if ((var)>(max)) (var)=(max); if ( (var)<-(max)) (var)=-(max);
    
void movePID( int pin, int *point, int *pointDest, struct counts *motor_cnt,int settleTime ) {

    int pointDelta, pointSpeed, pointLastDelta, pointDiff, pointInt;
    pointDelta = *pointDest - *point;
    pointLastDelta = pointInt = 0;

    while( settleTime ) {
        pointDelta = *pointDest - motor_cnt->cnt0;
        pointDiff = pointDelta - pointLastDelta;
        pointLastDelta = pointDelta;
        pointInt += pointDelta;
        BOUNDS ( pointInt , MOVEIMAX );
        pointSpeed = pointDelta * MOVEP + pointInt * MOVEI + pointDiff * MOVED;
        BOUNDS ( pointSpeed , MOVEMAX );
        analogWrite(pin, pointSpeed); 
        if ( pointDiff * pointDelta < 5)  settleTime --;
        //if ( pointDiff < 5)  settleTime --;
	delay(30);
    }

    return;

}

#ifdef __cplusplus
}
#endif
