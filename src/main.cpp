#include <Arduino.h>
#include <ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Empty.h>
#include <avr/delay.h>
#include <pid.h>
#include <pins.h>

//struct pins { int left; int right; int enable; int cnt0; int cnt1; };
struct pins motor_pins = { 10, 11, 4, 2, 3 };
volatile struct counts motor_cnt = { 0, 0 };
volatile int positionDelta, positionSpeed, positionLastDelta, positionDiff, positionInt = 0;
volatile int target_position = 0;
volatile int flag_0 = 0;
volatile int flag_1 = 0;

ros::NodeHandle nh;
std_msgs::Int32 click_msg;
ros::Publisher wheel_encoder_clicks("wheel_encoder_clicks", &click_msg);

// callback functions for Ros Subscribers
void drive_dist_cb ( const std_msgs::Int32& clicks ) {
        target_position += clicks.data;
}

void drive_to_cb ( const std_msgs::Int32& clicks ) {
        target_position = clicks.data;
}

void home_cb ( const std_msgs::Empty& toggle_msg ) {
        motor_cnt.cnt0 = 0;
        motor_cnt.cnt1 = 0;
        target_position = motor_cnt.cnt0;
}

void toggle_motor_cb ( const std_msgs::Empty& toggle_msg ) {
    target_position = motor_cnt.cnt0;
    digitalWrite(motor_pins.left, 0);
    digitalWrite(motor_pins.right, 0);
    digitalWrite(motor_pins.enable, !digitalRead(motor_pins.enable));
}

ros::Subscriber<std_msgs::Int32> drive_to("drive_to", &drive_to_cb );
ros::Subscriber<std_msgs::Int32> drive_distance("drive_dist", &drive_dist_cb );
ros::Subscriber<std_msgs::Empty> home("home", &home_cb );
ros::Subscriber<std_msgs::Empty> toggle_motor("toggle_motor", &toggle_motor_cb );


void count_encoder() {
    flag_0 = digitalRead(motor_pins.cnt0);
    flag_1 = digitalRead(motor_pins.cnt1);

    if ( flag_0 == flag_1 ) {
        motor_cnt.cnt0++;
    } else {
        motor_cnt.cnt0--;
    }

}

void setup()
{
    nh.initNode();
    nh.advertise(wheel_encoder_clicks);
    nh.subscribe(drive_to);
    nh.subscribe(drive_distance);
    nh.subscribe(home);
    nh.subscribe(toggle_motor);

    nh.getHardware()->setBaud(57600);
    attachInterrupt(digitalPinToInterrupt(motor_pins.cnt0), count_encoder, CHANGE);
    timer_init();

    pinMode(motor_pins.cnt0, INPUT);
    pinMode(motor_pins.cnt1, INPUT);
    pinMode(motor_pins.enable, OUTPUT);
    pinMode(motor_pins.left, OUTPUT);
    pinMode(motor_pins.right, OUTPUT);

    digitalWrite(motor_pins.left, 0);
    digitalWrite(motor_pins.right, 0);
    digitalWrite(motor_pins.enable, 1);
}

void loop()
{

    click_msg.data = motor_cnt.cnt0;
    wheel_encoder_clicks.publish( &click_msg );

    nh.spinOnce();
    _delay_ms(500);
}
