#include <Arduino.h>
//#include <std_msgs/Int32.h>
//#include <std_msgs/Empty.h>
//#include <avr/delay.h>
#include <pid.h>
#include <pins.h>

//struct pins { int left; int right; int enable; int cnt0; int cnt1; };
struct pins motor_pins = { 10, 11, 4, 2, 3 };
volatile int motor_cnt = 0; //position the motor ist at
volatile int positionDelta, positionSpeed, positionLastDelta, positionDiff, positionInt = 0; //PID variables
volatile int target_position = 0;
volatile int flag_0 = 0; //direction flag
volatile int flag_1 = 0; //direction flag

volatile int pin_hold = 5;  //activates PID
volatile int pin_fire = 6;  //sets target position to next position
volatile int pin_write = 7; //tells slave to read next position (from serial) or to post motor count (to serial)
volatile int pin_prime = 8; //lets slave use the serial bus

volatile int next_position = 0;


// commented out for use as templae for needed functions
//
// callback functions for Ros Subscribers
// void drive_dist_cb ( const std_msgs::Int32& clicks ) {
//         target_position += clicks.data;
// }
//
// void drive_to_cb ( const std_msgs::Int32& clicks ) {
//         target_position = clicks.data;
// }
//
// void home_cb ( const std_msgs::Empty& toggle_msg ) {
//         motor_cnt = 0;
//         target_position = motor_cnt;
// }
//
// void toggle_motor_cb ( const std_msgs::Empty& toggle_msg ) {
//     target_position = motor_cnt;
//     digitalWrite(motor_pins.left, 0);
//     digitalWrite(motor_pins.right, 0);
//     digitalWrite(motor_pins.enable, !digitalRead(motor_pins.enable));
// }

//count encoder steps function for interrupt
void count_encoder(){
    flag_0 = digitalRead(motor_pins.cnt0);
    flag_1 = digitalRead(motor_pins.cnt1);
    if ( flag_0 == flag_1 ) {
        motor_cnt++;
    } else {
        motor_cnt--;
    }
}

//clear serial input buffer
void serial_clear(){
  while (Serial.available() > 0) {
      Serial.read();
  }
}

// int serial_read_int(){
//   int byte_buffer[4];
//   int conv_integer;
//   Serial.readBytes(byte_buffer,4); //Read the serial data and store in var
//   //convert buffer to conv_integer
//   //TODO
//   conv_integer = 0; //somthing to avoid errors
//   return integer;
// }

void setup(){
    //Set the Serialport to 9600 Baud (other Bauds are possible, up to 115200)
    Serial.begin(9600);

    //start interrupt for counting the encoder steps
    attachInterrupt(digitalPinToInterrupt(motor_pins.cnt0), count_encoder, CHANGE);

    //start timer interrupt for PID controller
    timer_init();

    //initialise pins
    pinMode(motor_pins.cnt0, INPUT);
    pinMode(motor_pins.cnt1, INPUT);
    pinMode(motor_pins.enable, OUTPUT);
    pinMode(motor_pins.left, OUTPUT);
    pinMode(motor_pins.right, OUTPUT);
    pinMode(pin_hold, INPUT);
    pinMode(pin_fire, INPUT);
    pinMode(pin_write, INPUT);
    pinMode(pin_prime, INPUT);


    digitalWrite(motor_pins.left, 0);
    digitalWrite(motor_pins.right, 0);
    digitalWrite(motor_pins.enable, 1);

    //Home
    //Some code to make sure robot is in Home Position and all encoders are 0
    //TODO
}

void loop()
{
    if (digitalRead(pin_prime)){
      if (digitalRead(pin_write)) {
        //clear serial buffer
        serial_clear();
        //send ready byte
        Serial.write(255);
        //read next position from serial (4 bytes)
        next_position = Serial.read(); //placeholder, leter the function int serial_read_int() shold be called
      }else{
        Serial.write(motor_cnt);
      }
    }

    if (!digitalRead(pin_hold)) {
      //deactivate PID
      //TODO
    }

    if (digitalRead(pin_fire)) {
      target_position = next_position;
    }
}
