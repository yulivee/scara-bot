#include <Arduino.h>
#include <pins.h>
#include <pid.h>

#ifdef __cplusplus
extern "C" {
#endif

void stop_motors (struct pins *axis) {
    digitalWrite(axis->left, 0 );
    digitalWrite(axis->right, 0 );
    digitalWrite(axis->enable, 0);
}

void move ( int *dist, struct pins *axis, struct counts *count, int direction ) {

    int current_pos = count->cnt0;
    int target_pos = 0;

    if ( direction == RIGHT ) {
        target_pos = current_pos + *dist;
        movePID( axis->right, &current_pos, &target_pos, 1 );
    } else {
        target_pos = current_pos - *dist;
        movePID( axis->left,  &current_pos, &target_pos, 1 );
    }

    stop_motors(axis);

}

#ifdef __cplusplus
}
#endif
