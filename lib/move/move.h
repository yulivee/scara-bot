#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

int clicks = 30;    
int tempo = 30;
void stop_motors (struct pins *axis);
void move ( struct pins *axis, struct counts *count, int direction );

#ifdef __cplusplus
}
#endif

#endif
