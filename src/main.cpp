#include <Arduino.h>
//#include <std_msgs/Int32.h>
//#include <std_msgs/Empty.h>
//#include <avr/delay.h>
#include <pid.h>
#include <pins.h>

String slave_name = "Slave 1";

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
volatile int pin_prime = 9; //lets slave use the serial bus

volatile int pin_led = 13;

volatile word next_position = 0;
volatile bool pin_toggled_high = true;

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
  pinMode(pin_led, OUTPUT);

  //make sure all motors are off, activate drivers are active
  digitalWrite(motor_pins.left, 0);
  digitalWrite(motor_pins.right, 0);
  digitalWrite(motor_pins.enable, 1);
  digitalWrite(pin_led, 0);
}

void loop()
{
  if (digitalRead(pin_prime)){  //prime signal tells Slave to send or receive data on the serial bus

    if (pin_toggled_high) {     //check that sigal has been toggled from off to on
      pin_toggled_high = false;

      if (digitalRead(pin_write)) {  //write to bus
        Serial.println("this is " + slave_name);
      }
      else{//read from bus
        serial_clear();         //clear serial buffer
        //send ready signal
        Serial.println(slave_name + "ready");
        //read char from serial
        char serial_data = Serial.read();
        if (serial_data == 'h'){
          //turn on LED
          digitalWrite(pin_led, 1);
        }else if (serial_data == 'l')
      {
        //turn off LED
        digitalWrite(pin_led, 0);
      }
      }
    }
  }
  else{ //reset variable for recognising off-on toggle
    pin_toggled_high = true;
  }

  if (!digitalRead(pin_hold)) {
    //deactivate motors
    digitalWrite(motor_pins.enable, 1);
    target_position = motor_cnt;
  }

  if (digitalRead(pin_fire)) {
    target_position = next_position;
  }

}
