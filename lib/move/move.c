#include <Arduino.h>
#ifdef __cplusplus
extern "C" {
#endif

void stop_motors (struct pins *axis) {
    digitalWrite(axis->left, 0 );
    digitalWrite(axis->right, 0 );
    digitalWrite(axis->enable, 0);
}

void move ( struct pins *axis, struct counts *count, int direction ) {

    Serial.println(counts->cnt0);
    int target_count = counts->cnt0;
    int start_value = counts->cnt0;

    Serial.print(">> ");
    Serial.print(counts->cnt0);
    Serial.println(" <<");
    digitalWrite(axis->enable, 1);

	if ( direction == RIGHT ) {
	    target_count += clicks;
	    while ( target_count > counts->cnt0 && counts->cnt0 != 0 ) {
		    digitalWrite(axis->right, 1);
	    }
	} else {
	    target_count -= clicks;
	    while ( target_count < counts->cnt0 && counts->cnt0 != 0 ) {
		    digitalWrite(axis->left, 1);
	    }
	}

    stop_motors(axis);

    Serial.print(">> ");
    Serial.print(counts->cnt0);
    Serial.print(" >> ");
    Serial.print(counts->cnt0 - start_value);
    Serial.println(" <<");


}

#ifdef __cplusplus
}
#endif
