#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

void stop_motors (struct pins *axis);
void move ( int *dist,  struct pins *axis, struct counts *count, int direction );

#ifdef __cplusplus
}
#endif

#endif
