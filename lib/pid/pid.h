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
    
void movePID( int pointDest, uint8_t settleTime );

#ifdef __cplusplus
}
#endif

#endif
