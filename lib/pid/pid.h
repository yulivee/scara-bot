#ifndef PID_H_INCLUDED
#define PID_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

#define MOVEP 3
#define MOVEI 0.1
#define MOVED 6
#define MOVEMAX 25
#define MOVEIMAX 200    
#define BOUNDS(var, max) if ((var)>(max)) (var)=(max); if ( (var)<-(max)) (var)=-(max);
    
void movePID( int pointDest, uint8_t settleTime ) {

    int pointDelta, pointSpeed, pointLastDelta, pointDiff, pointInt;
    pointLastDelta = pointInt = 0;

    while( settleTime ) {
        pointDelta = pointDest - point;
        pointDiff = pointDelta - pointLastDelta;
        pointLastDelta = pointDelta;
        pointInt += pointDelta;
        BOUNDS ( pointInt , MOVEIMAX );
        pointSpeed = pointDelta * MOVEP + pointInt * MOVEI + pointDiff * MOVE;
        BOUNDS ( pointSpeed , MOVEMAX );
        //CTMotor ( pointSpeed - angSpeed , pointSpeed + angSpeed ) ;
        if ( pointDelta * pointDelta < 5)  ) settleTime --;
        //_delay_ms (30) ;
    }

    return;

}

#ifdef __cplusplus
}
#endif

#endif
