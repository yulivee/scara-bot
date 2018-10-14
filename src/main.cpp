#include <Arduino.h>
//#include <std_msgs/Int32.h>
//#include <std_msgs/Empty.h>
//#include <avr/delay.h>
#include <pid.h>
#include <pins.h>

// -------------------------------
// VARIABLES
// -------------------------------
const String slave_name = "Slave 1";
const int slave_number = 0;
//String slave_name = "Slave 2";
volatile int motor_cnt = 0; //position the motor ist at
volatile int positionDelta, positionSpeed, positionLastDelta, positionDiff, positionInt = 0; //PID variables
volatile int target_position = 0;
volatile int flag_0 = 0; //direction flag
volatile int flag_1 = 0; //direction flag
volatile word next_position = 0;
volatile bool pin_toggled_high = true;

// -------------------------------
//PINOUT:
// -------------------------------
volatile int pin_hold = 5;  //Input: activates PID, normal HIGH
volatile int pin_fire = 6;  //Input: sets target position to next position, normal LOW
volatile int pin_write = 7; //Input: allows slave to write to serial bus, normal LOW
int debug_pin = 8;
volatile int pin_prime = 9; //Input: lets slave use the serial bus, normal LOW, private slave signal
volatile int pin_led1 = 13; //Output: onboard LED
volatile int pin_led2 = 12; //Output: offboard LED
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
// void toggle_motor_cb ( const std_msgs::Empty& toggle_msg ) {
//     target_position = motor_cnt;
//     digitalWrite(motor_pins.left, 0);
//     digitalWrite(motor_pins.right, 0);
//     digitalWrite(motor_pins.enable, !digitalRead(motor_pins.enable));
// }

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

// TODO
void ping(){
  serial_write_int(slave_number);
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
  pinMode(pin_hold, INPUT);
  pinMode(pin_fire, INPUT);
  pinMode(pin_write, INPUT);
  pinMode(pin_prime, INPUT);
  pinMode(pin_led1, OUTPUT);
  pinMode(pin_led2, OUTPUT);
  pinMode(debug_pin, OUTPUT);

  //make sure all motors are off, activate drivers are active
  digitalWrite(motor_pins.left, 0);
  digitalWrite(motor_pins.right, 0);
  digitalWrite(motor_pins.enable, 1);
  digitalWrite(pin_led1, 1);
  digitalWrite(pin_led2, 1);
  delay(500);
  digitalWrite(pin_led1, 0);
  digitalWrite(pin_led2, 0);
  digitalWrite(debug_pin, 0);
}

void loop()
{
  // read from port 0, send to port 1:
  //if (Serial.available()) {
  //  digitalWrite(pin_led1, 1);
  //  Serial.write(Serial.read());
//    digitalWrite(pin_led1, 0);
//  }

  if (digitalRead(pin_prime)){  //prime signal tells Slave to send or receive data on the serial bus

    if (pin_toggled_high) {     //check that sigal has been toggled from off to on
      pin_toggled_high = false;
      digitalWrite(pin_led1, 1);

      if (digitalRead(pin_write)) {  //write to bus
        Serial.println("this is " + slave_name);
      }
      else{//read from bus
        serial_clear();         //clear serial buffer
        //send ready signal
        Serial.println(slave_name + " ready");
        digitalWrite(debug_pin,1);

        while (Serial.available() == 0) {
          delay(100);
        }
        //read char from serial
        digitalWrite(debug_pin,0);
        int serial_data = serial_read_int();
        Serial.println(slave_name + " received " + serial_data);

/*
        digitalWrite(pin_led1, 0);
        if (serial_data == 'h'){
          //turn on LED
          digitalWrite(pin_led2, 1);
        }else if (serial_data == 'l')
      {
        //turn off LED
        digitalWrite(pin_led2, 0);
      } */
      }
    }
  }
  else{ //reset variable for recognising off-on toggle
    pin_toggled_high = true;
    digitalWrite(pin_led1, 0);
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
