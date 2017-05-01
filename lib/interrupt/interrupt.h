#ifndef INTERRUPT_H_INCLUDED
#define INTERRUPT_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

volatile int flag = 0;
void doCount0();
void doCount1();

#ifdef __cplusplus
}
#endif


#endif
