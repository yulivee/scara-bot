#ifndef PINS_H_INCLUDED
#define PINS_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

#define RIGHT 1
#define LEFT 0	

struct pins { int left; int right; int enable; };

struct pins shoulder = { 10, 11, 14 };
struct pins ske = { 4,5,0 };
struct pins skg = { 12, 13, 0 };
struct pins skf = { 2, 3 ,0 };
struct pins uaj= { 7, 6, 0 };
struct pins uae = { 9, 8, 0 };
struct pins zaxis = { 44, 45, 0 };


#ifdef __cplusplus
}
#endif

#endif

