
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
const int slave_number = 7;

volatile int motor_cnt = 0; //position the motor ist at
volatile int positionSpeed = 0;
volatile int positionDelta, positionLastDelta, positionDiff, positionInt = 0; //PID variables
volatile int motorSpeed,speedDelta = 0;
int maxMotorSpeed = 255;
int target_position = 0;
volatile int flag_0 = 0; //direction flag
volatile int flag_1 = 0; //direction flag
bool last_prime_state = 0;
bool last_fire_state = 0;
volatile int zone = 5;  //fly-by distance to point in clicks

enum Command {
  c_ping = 0,
  c_home = 1,
  c_set_pid_state = 2,
  c_get_position = 6,
  c_get_target = 7,
  c_get_slave_num =8,
  c_drive_dist = 10,
  c_drive_dist_max = 11,
  c_drive_to = 12,
  c_set_speed = 15,
  c_set_zone = 16,
  c_check_target_reached = 20
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

// -------------------------------
//PID FUNCTIONS
// -------------------------------

//turn PID on or off
void set_pid_state ( bool motor_state ) {
  target_position = motor_cnt;
  digitalWrite(motor_pins.left, 0);
  digitalWrite(motor_pins.right, 0);
  digitalWrite(motor_pins.enable, motor_state);
}

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

// -------------------------------
//SERIAL FUNCTIONS
// -------------------------------

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
// MAIN CODE
// -------------------------------
void setup(){
  //Set the Serial port to 9600 Baud for communication with Master (other Bauds are possible, up to 115200)
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

  //make sure all motors are off, activate drivers
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
  //---------------------------------------------------------------------------
  //PRIMING: prime signal tells Slave to send or receive data on the serial bus
  int prime_state = digitalRead(ss_pin);
  // check if master has set priming signal to high (edge detection)
  if (prime_state == 1 && last_prime_state == 0){
    //---------------------------------------------------------------------------
    //START SERIAL COMMUNICATION:
    digitalWrite(led_pin, 1); //show that priming is active
    serial_clear();           //clear serial buffer
    serial_write_int(slave_number);       //send ready signal

    //---------------------------------------------------------------------------
    //RECEIVE COMMAND: wait for command pakage from master
    while (Serial.available() == 0) {
      delayMicroseconds(50);
      //TODO implement Timeout
    }
    //read command bytes from Serial
    Command command = (Command)serial_read_int();

    //---------------------------------------------------------------------------
    //EXECUTE COMMAND:
    int data; //variable for holding data from the master
    int dist_to_target; //variable for computing current distance to target
    bool target_reached;  //result boolean for checking if target reached
    int error_code = command; //set error_code to command number unless an error ocurrs

    switch (command) {
      case c_ping:       //echo received data to bus
      data = serial_read_int(); //read data bytes from serial
      serial_write_int(data);   //echo bytes
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
        error_code=e_bad_data;
      }
      break;

      case c_get_position:  // send current position to bus
      serial_write_int(motor_cnt);
      break;

      case c_get_target:  // send current target to bus
      serial_write_int(target_position);
      break;

      case c_get_slave_num:
      serial_write_int(slave_number);
      break;

      case c_drive_dist:  // prepare to increment the target position by received ammount
      data = serial_read_int();
      target_position=target_position + data;
      break;

      case c_drive_dist_max: // set target position to actual pSosition incremented by received data (asynchronous drive!)
      data = serial_read_int();
      target_position = motor_cnt + data;
      break;

      case c_drive_to:  // set the next position
      data = serial_read_int();
      target_position = data;
      break;

      case c_set_speed:  // set the axis max speed
      data = serial_read_int();
      if ((0 <= data) && (data <= 255)) {
        maxMotorSpeed = data;
      }else{
        error_code=e_bad_data;
      }
      break;

      case c_set_zone:  // set the fly-by distance
      zone = serial_read_int();
      break;

      case c_check_target_reached:  // tell master if target position is reached (within zone)
      dist_to_target = target_position-motor_cnt; //calculate distance to target
      target_reached = false;
      if ((-zone<dist_to_target) && dist_to_target<zone) { //if distance is within zone boundraries set result to true
        target_reached = true;
      }
      serial_write_int((int)target_reached); //as communication only works with ints, typecast result as int
      break;

      default:
      error_code = e_unknown_command;
      break;
    }

    //---------------------------------------------------------------------------
    //END SERIAL COMMUNICATION:
    while (digitalRead(ss_pin)) { //Wait for slave select to go LOW
    }
    serial_write_int(error_code); //Send error code to Master
    digitalWrite(led_pin, 0);     //show that priming and command execution has ended
  }
  last_prime_state=prime_state; //necessary for edge detection
}
