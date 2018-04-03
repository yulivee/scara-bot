#include <Arduino.h>
#include <ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Empty.h>
#include <avr/delay.h>
#include <pid.h>
#include <pins.h>

//struct pins { int left; int right; int enable; int cnt0; int cnt1; };
struct pins motor_pins = { 10, 11, 4, 2, 3 };
volatile int motor_cnt = 0;
volatile struct pid_position pid_position = { 0, 0, 0, 0, 0 };
volatile struct pid_parameters pid_parameters = { 15, 0.6, 12, 255, 100, 100 };
volatile int positionDelta, positionSpeed, positionLastDelta, positionDiff, positionInt = 0;
volatile int target_position = 0;
volatile int flag_0 = 0;
volatile int flag_1 = 0;

ros::NodeHandle nh;
std_msgs::Int32 click_msg;
std_msgs::Int32 test_msg;
ros::Publisher wheel_encoder_clicks("wheel_encoder_clicks", &click_msg);
ros::Publisher test("test", &test_msg);

// callback functions for Ros Subscribers
void drive_dist_cb ( const std_msgs::Int32& clicks ) {
        target_position += clicks.data;
}

void drive_to_cb ( const std_msgs::Int32& clicks ) {
        target_position = clicks.data;
}

void home_cb ( const std_msgs::Empty& toggle_msg ) {
        motor_cnt = 0;
        target_position = motor_cnt;
}

void toggle_motor_cb ( const std_msgs::Empty& toggle_msg ) {
    target_position = motor_cnt;
    digitalWrite(motor_pins.left, 0);
    digitalWrite(motor_pins.right, 0);
    digitalWrite(motor_pins.enable, !digitalRead(motor_pins.enable));
}

//define Ros subscribers
ros::Subscriber<std_msgs::Int32> drive_to("drive_to", &drive_to_cb );
ros::Subscriber<std_msgs::Int32> drive_distance("drive_dist", &drive_dist_cb );
ros::Subscriber<std_msgs::Empty> home("home", &home_cb );
ros::Subscriber<std_msgs::Empty> toggle_motor("toggle_motor", &toggle_motor_cb );

//count encoder steps function for interrupt
void count_encoder() {
    flag_0 = digitalRead(motor_pins.cnt0);
    flag_1 = digitalRead(motor_pins.cnt1);

    if ( flag_0 == flag_1 ) {
        motor_cnt++;
    } else {
        motor_cnt--;
    }

}

void setup()
{
    //Initialise Ros Node, publisher and subsribers
    nh.initNode();
    nh.advertise(wheel_encoder_clicks);
    nh.advertise(test);
    nh.subscribe(drive_to);
    nh.subscribe(drive_distance);
    nh.subscribe(home);
    nh.subscribe(toggle_motor);



    //Set baud rate for Ros serial communication
    nh.getHardware()->setBaud(57600);

    //start interrupt for counting the encoder steps
    attachInterrupt(digitalPinToInterrupt(motor_pins.cnt0), count_encoder, CHANGE);

    //start timer interrupt for PID controller
    timer_init(&pid_parameters->govenor_freq);

    //initialise pins
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
    // wait until the node handle has connected to ROS
    while(!nh.connected()) {nh.spinOnce();}
    int test_value;
    if (! nh.getParam("/scara/uaj/pid/p", &test_value, 1 )) {
        // default values
        test_value = 0;
    }

    test_msg.data = test_value;
    //publish clicks to Ros
    click_msg.data = motor_cnt;
    wheel_encoder_clicks.publish( &click_msg );
    test.publish( &test_msg );

    //cyclical communication with Ros Master
    nh.spinOnce();
    _delay_ms(500);
}
