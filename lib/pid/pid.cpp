#include <Arduino.h>
#include <HardwareSerial.h>
#include <pins.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MOVEP 0.3
#define MOVEI 0.1 //0.5
#define MOVED 0.6
#define MOVEMAX 255 //25
#define MOVEIMAX 200    //
#define BOUNDS(var, max) if ((var)>(max)) (var)=(max); if ( (var)<-(max)) (var)=-(max);
    
void movePID( int pin, int *point, int *pointDest, struct counts *motor_cnt,int settleTime ) {

    Serial.begin(9600);
    int pointDelta, pointSpeed, pointLastDelta, pointDiff, pointInt;
    pointDelta = *pointDest - *point;
    pointLastDelta = pointInt = 0;

    while( settleTime ) {
	Serial.println(motor_cnt->cnt0);
        pointDelta = *pointDest - motor_cnt->cnt0;
	Serial.print("pointDelta ");
	Serial.println(pointDelta);
        pointDiff = pointDelta - pointLastDelta;
	Serial.print("pointDiff ");
	Serial.println(pointDiff);
	Serial.print("pointLastDelta ");
	Serial.println(pointLastDelta);
        pointLastDelta = pointDelta;
	Serial.print("pointLastDelta ");
	Serial.println(pointLastDelta);
        pointInt += pointDelta;
	Serial.print("pointInt ");
	Serial.println(pointInt);
        BOUNDS ( pointInt , MOVEIMAX );
	Serial.print("pointInt ");
	Serial.println(pointInt);
        pointSpeed = pointDelta * MOVEP + pointInt * MOVEI + pointDiff * MOVED;
	Serial.print("pointSpeed ");
	Serial.println(pointSpeed);
        BOUNDS ( pointSpeed , MOVEMAX );
	Serial.print("pointSpeed ");
	Serial.println(pointSpeed);
        analogWrite(pin, pointSpeed); 
        //if ( pointDiff * pointDelta < 5)  settleTime --;
        //if ( pointDiff < 5)  settleTime --;
	delay(30);
    }

    return;

}

#ifdef __cplusplus
}
#endif
