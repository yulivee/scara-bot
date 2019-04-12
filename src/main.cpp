
// --------------------------------------
// Name: main.cpp
// Project: scara-bot
// Description: main function for the arduino nano slaves on the multiserial bus
//---------------------------------------

#include <Arduino.h>
//#include <std_msgs/Int32.h>
//#include <std_msgs/Empty.h>
//#include <avr/delay.h>
#include <pid.h>
#include <pins.h>

// -------------------------------
// VARIABLES
// -------------------------------
const int slave_number = 6;

volatile int motor_cnt = 0; //position the motor ist at
volatile int positionSpeed = 0;
volatile int positionDelta, positionLastDelta, positionDiff, positionInt = 0; //PID variables
int target_position = 0;
volatile int flag_0 = 0; //direction flag
volatile int flag_1 = 0; //direction flag
bool last_prime_state = 0;
bool last_fire_state = 0;
enum Command {
  c_ping = 0,
  c_home = 1,
  c_set_pid_state = 5,
  c_get_position = 6,
  c_get_target =7,
  c_get_slave_num =8,
  c_drive_dist = 10,
  c_drive_dist_max = 11,
  c_drive_to = 12
};

//possible errors
enum Errortype {
  no_error = 0,
  e_wrong_slave = 91,
  e_ping_bad_echo = 92,
  e_unknown_command = 93,
  e_bad_data = 94,
  default_value = 99
};

// -------------------------------
//PINOUT:
// -------------------------------
volatile int ss_pin = 12; //Slave Select pin, Input: lets slave use the serial bus, normal LOW, private slave signal
volatile int led_pin = 13; //Output: onboard LED
struct pins motor_pins = { 10, 11, 4, 2, 3 }; //struct pins { int left; int right; int enable; int cnt0; int cnt1; };

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
void set_pid_state ( bool motor_state ) {
  target_position = motor_cnt;
  digitalWrite(motor_pins.left, 0);
  digitalWrite(motor_pins.right, 0);
  digitalWrite(motor_pins.enable, motor_state);
}

// -------------------------------
//FUNCTIONS
// -------------------------------
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

int serial_read_int(){
  byte byte_buffer[2];
  Serial.readBytes(byte_buffer,2); //Store the next 2 Bytes of serial data in the buffer
  //convert buffer to conv_integer
  int val = ((byte_buffer[1]) << 8) + byte_buffer[0];
  return val;
}

void serial_write_int(int val){
  Serial.write(lowByte(val));
  Serial.write(highByte(val));
}

// -------------------------------
// MAIN
// -------------------------------
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
  pinMode(ss_pin, INPUT);
  pinMode(led_pin, OUTPUT);

  //make sure all motors are off, activate drivers are active
  digitalWrite(motor_pins.left, 0);
  digitalWrite(motor_pins.right, 0);
  digitalWrite(motor_pins.enable, 1);

  //blink leds
  digitalWrite(led_pin, 1);
  delay(500);
  digitalWrite(led_pin, 0);
}

void loop()
{


  //prime signal tells Slave to send or receive data on the serial bus
  int prime_state = digitalRead(ss_pin);
  // check if master has set priming signal to high (edge detection)
  if (prime_state == 1 && last_prime_state == 0){
    //show that priming is active
    digitalWrite(led_pin, 1);
    //clear serial buffer
    serial_clear();
    //send ready signal
    serial_write_int(slave_number);
    //wait for command pacckage from master
    while (Serial.available() == 0) {
      delayMicroseconds(50);
      //TODO implement Timout!
    }
    //read command bytes from Serial
    Command command = (Command)serial_read_int();

    int data; //variable for holding data from the master
    int return_data = default_value; //variable for answering data to master

    //echo command number unless an error ocurrs
    return_data = command;

    //execute command
    switch (command) {
      case c_ping:       //echo received data to bus
      //read data bytes from serial
      data = serial_read_int();
      return_data = data;
      break;

      case c_home:      // set current position as home, by zeroing counters
      set_pid_state(false);
      motor_cnt= 0;
      target_position = 0;
      set_pid_state(true);
      break;

      case c_set_pid_state: // set state of the PID
      data = serial_read_int();
      if (data == 1) {
        set_pid_state(true);
      }else if (data == 0) {
        set_pid_state(false);
      }else{
        return_data=e_bad_data;
      }
      break;

      case c_get_position:  // send current position to bus
      return_data = motor_cnt;
      break;

      case c_get_target:  // send current target to bus
      return_data = target_position;
      break;

      case c_get_slave_num:
      return_data = slave_number;
      break;

      case c_drive_dist:  // prepare to increment the target position by received ammount
      data = serial_read_int();
      target_position=target_position + data;
      return_data = target_position;
      break;

      case c_drive_dist_max: // set target position to actual pSosition incremented by received data (asynchronous drive!)
      data = serial_read_int();
      target_position = motor_cnt + data;
      return_data = target_position;
      break;

      case c_drive_to:  // set the next position
      data = serial_read_int();
      target_position = data;
      return_data = target_position;
      break;

      default:
      return_data = e_unknown_command;
      break;
    }
    serial_write_int(return_data);
    //show that priming and command execution has ended
    digitalWrite(led_pin, 0);
  }
  last_prime_state=prime_state; //necessary for edge detection
}
