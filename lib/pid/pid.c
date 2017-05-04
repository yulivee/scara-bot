#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MOVEP 3
#define MOVEI 0.1
#define MOVED 6
#define MOVEMAX 25
#define MOVEIMAX 200    
#define BOUNDS(var, max) if ((var)>(max)) (var)=(max); if ( (var)<-(max)) (var)=-(max);
    
void movePID( int pin, int *point, int *pointDest, int settleTime ) {

    int pointDelta, pointSpeed, pointLastDelta, pointDiff, pointInt;
    pointLastDelta = pointInt = 0;

    while( settleTime ) {
        pointDelta = pointDest - point;
        pointDiff = pointDelta - pointLastDelta;
        pointLastDelta = pointDelta;
        pointInt += pointDelta;
        BOUNDS ( pointInt , MOVEIMAX );
        pointSpeed = pointDelta * MOVEP + pointInt * MOVEI + pointDiff * MOVED;
        BOUNDS ( pointSpeed , MOVEMAX );
        analogWrite(pin, pointSpeed); 
        if ( pointDelta * pointDelta < 5)  settleTime --;
    }

    return;

}

#ifdef __cplusplus
}
#endif
