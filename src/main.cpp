#include <Arduino.h>
int links_ske = 5;
int rechts_ske = 4;
int links_skf = 2;
int rechts_skf = 3;
int links_skg = 12;
int rechts_skg = 13;
int links_uaj = 7;
int rechts_uaj = 6 ;
int links_uae = 9;
int rechts_uae = 8;
int links_shoulder = 10;
int rechts_shoulder = 11;
int shoulder_enable = 14;
int links_z = 44;
int rechts_z = 45;
volatile int cnt0 = 0;
volatile int cnt1 = 0;
int monitor = 0;

int tempo = 30;
int clicks = 100;

void doCount0() { cnt0++; }
void doCount1() { cnt1++;  }

void setup()
{
    Serial.begin(9600);
    //pinMode(enable1, OUTPUT);
    //digitalWrite(enable1, HIGH);
    attachInterrupt(digitalPinToInterrupt(20), doCount0, CHANGE); // Interrupt 4 liegt auf Pin 19
    attachInterrupt(5, doCount1, CHANGE); // Interrupt 5 liegt auf Pin 18
    pinMode(shoulder_enable, OUTPUT);
    pinMode(links_shoulder, OUTPUT);
    pinMode(rechts_shoulder, OUTPUT);
    pinMode(links_uae, OUTPUT);
    pinMode(rechts_uae, OUTPUT);
    pinMode(links_uaj, OUTPUT);
    pinMode(rechts_uaj, OUTPUT);
    pinMode(links_skg, OUTPUT);
    pinMode(rechts_skg, OUTPUT);
    pinMode(links_skf, OUTPUT);
    pinMode(rechts_skf, OUTPUT);
    pinMode(links_ske, OUTPUT);
    pinMode(rechts_ske, OUTPUT);
    pinMode(links_z, OUTPUT);
    pinMode(rechts_z, OUTPUT);

    analogWrite(links_shoulder, 0);
    analogWrite(rechts_shoulder, 0);
    analogWrite(links_uae, 0);
    analogWrite(rechts_uae, 0);
    analogWrite(links_uaj, 0);
    analogWrite(rechts_uaj, 0);
    analogWrite(links_skg, 0);
    analogWrite(rechts_skg, 0);
    analogWrite(links_skf, 0);
    analogWrite(rechts_skf, 0);
    analogWrite(links_ske, 0);
    analogWrite(rechts_ske, 0);
    analogWrite(links_z, 0);
    analogWrite(rechts_z, 0);
}

void stop_motors () {
    analogWrite(links_shoulder, 0);
    analogWrite(rechts_shoulder, 0);
    analogWrite(links_uae, 0);
    analogWrite(rechts_uae, 0);
    analogWrite(links_uaj, 0);
    analogWrite(rechts_uaj, 0);
    analogWrite(links_skg, 0);
    analogWrite(rechts_skg, 0);
    analogWrite(links_skf, 0);
    analogWrite(rechts_skf, 0);
    analogWrite(links_ske, 0);
    analogWrite(rechts_ske, 0);
    analogWrite(links_z, 0);
    analogWrite(rechts_z, 0);
}

void move_z_right () {
    analogWrite(links_z, tempo);
    analogWrite(rechts_z, 0 );
    delay(1000);
    stop_motors();
}

void move_z_left () {
    analogWrite(links_z, 0);
    analogWrite(rechts_z, tempo );
    delay(1000);
    stop_motors();
}

void move_shoulder_right () {

    digitalWrite(shoulder_enable, 1);

    Serial.println(cnt0);
    int current_enc = cnt0;
    int start_value = cnt0;
    current_enc += clicks;
	Serial.print(">> ");
	Serial.print(cnt0);
	Serial.println(" <<");
    
    while ( current_enc > cnt0 ) {
	analogWrite(rechts_shoulder, tempo);
	Serial.print("-- ");
	Serial.print(cnt0);
	Serial.println(" --");
    }
	digitalWrite(links_shoulder, 0 );
	digitalWrite(rechts_shoulder, 0 );
	digitalWrite(shoulder_enable, 0);

	Serial.print(">> ");
	Serial.print(cnt0);
	Serial.print(" >> ");
	Serial.print(cnt0 - start_value);
	Serial.println(" <<");

    stop_motors();
}

void move_shoulder_left () {
    analogWrite(links_shoulder, 0);
    analogWrite(rechts_shoulder, tempo );
    delay(1000);
    stop_motors();
}

void move_uae_right () {
    analogWrite(links_uae,tempo);
    analogWrite(rechts_uae, 0);
    delay(1000);
    stop_motors();
}

void move_uae_left () {
    analogWrite(links_uae, 0);
    analogWrite(rechts_uae, tempo);
    delay(1000);
    stop_motors();
}

void move_uaj_right () {
    analogWrite(links_uaj,tempo);
    analogWrite(rechts_uaj, 0);
    delay(1000);
    stop_motors();
}

void move_uaj_left () {
    analogWrite(links_uaj, 0);
    analogWrite(rechts_uaj, tempo);
    delay(1000);
    stop_motors();
}

void move_ske_right () {
    analogWrite(links_ske,tempo);
    analogWrite(rechts_ske, 0);
    delay(1000);
    stop_motors();
}

void move_ske_left () {
    analogWrite(links_ske, 0);
    analogWrite(rechts_ske, tempo);
    delay(1000);
    stop_motors();
}

void move_skg_right () {
    analogWrite(links_skg,tempo);
    analogWrite(rechts_skg, 0);
    delay(1000);
    stop_motors();
}

void move_skg_left () {
    analogWrite(links_skg, 0);
    analogWrite(rechts_skg, tempo);
    delay(1000);
    stop_motors();
}

void move_skf_right () {
    analogWrite(links_skf,tempo);
    analogWrite(rechts_skf, 0);
    delay(1000);
    stop_motors();
}

void move_skf_left () {
    analogWrite(links_skf, 0);
    analogWrite(rechts_skf, tempo);
    delay(1000);
    stop_motors();
}

void loop()
{

    if (Serial.available() > 0) {
        monitor = Serial.read();

        if( monitor == 49 ) { // aus
            stop_motors();
        }

        if( monitor == 113 ) {
            move_shoulder_right();
            monitor = 49;
        }

        if( monitor == 119 ) {
            move_shoulder_left();
            monitor = 49;
        }
        if( monitor == 97 ) {
            move_uae_left();
            monitor = 49;
        }
        if ( monitor == 115 ) {
            move_uae_right();
            monitor = 49;

        }
        if ( monitor == 121 ) {
            move_uaj_right();
            monitor = 49;

        }
        if ( monitor == 120 ) {
            move_uaj_left();
            monitor = 49;

        }
	if ( monitor == 101 ) {
	    move_ske_right();
            monitor = 49;
	}
	if ( monitor == 114 ) {
	    move_ske_left();
            monitor = 49;
	}
	if ( monitor == 100 ) {
	    move_skf_right();
            monitor = 49;
	}
	if ( monitor == 102 ) {
	    move_skf_left();
            monitor = 49;
	}
	if ( monitor == 99 ) {
	    move_skg_right();
            monitor = 49;
	}
	if ( monitor == 118 ) {
	    move_skg_left();
            monitor = 49;
	}
	if ( monitor == 116 ) {
		move_z_left();
		monitor = 49;
	}
	if ( monitor == 122 ) {
		move_z_right();
		monitor = 49;
	}
	if ( monitor == 111 ) {
		tempo-=5;
		Serial.print ("Neues Tempo: ");
		Serial.println( tempo );
	}
	if ( monitor == 112 ) {
		tempo+=5;
		Serial.print ("Neues Tempo: ");
		Serial.println( tempo );
	}
	if ( monitor == 107 ) {
		clicks-=5;
		Serial.print ("Anzahl Schritte: ");
		Serial.println( clicks );
	}
	if ( monitor == 108 ) {
		clicks+=5;
		Serial.print ("Anzahl Schritte: ");
		Serial.println( clicks );
	}
        Serial.println( monitor );
    }
    delay(1000);
}

