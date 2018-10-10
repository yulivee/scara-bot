#include <Arduino.h>
//#include <std_msgs/Int32.h>
//#include <std_msgs/Empty.h>
//#include <avr/delay.h>
#include <pid.h>
#include <pins.h>

word serial_read_2b_word(){
  byte byte_buffer[2];
  Serial.readBytes(byte_buffer,2); //Store the next 2 Bytes of serial data in the buffer
  //convert buffer to conv_integer
  word val = ((byte_buffer[1]) << 8) + byte_buffer[0];
  return val;
}

void serial_write_2b_word(word val){
  Serial.write(lowByte(val));
  Serial.write(highByte(val));
}
