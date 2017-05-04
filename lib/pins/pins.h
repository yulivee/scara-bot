#ifndef PINS_H_INCLUDED
#define PINS_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

#define RIGHT 1
#define LEFT 0

struct pins { int left; int right; int enable; int cnt0; int cnt1; };
struct counts { volatile int cnt0; volatile int cnt1; };

#ifdef __cplusplus
}
#endif

#endif
