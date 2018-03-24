#include <Arduino.h>
#include <ros.h>
#include <std_msgs/Int8.h>
#include <avr/delay.h>
#include <pid.h>
#include <pins.h>
int monitor = 0;
int clicks = 300;

//struct pins { int left; int right; int enable; int cnt0; int cnt1; };
struct pins motor_pins = { 10, 11, 4, 2, 3 };
volatile struct counts motor_cnt = { 0, 0 };
volatile int positionDelta, positionSpeed, positionLastDelta, positionDiff, positionInt = 0;
volatile int target_position = 0;
volatile int current_position = 0;
volatile int flag_0 = 0;
volatile int flag_1 = 0;

ros::NodeHandle nh;
/* Non-working ROS-Publisher
std_msgs::Int8 direction_msg;
ros::Publisher wheel_encoder_clicks("wheel_encoder_clicks", &click_msg);
ros::Publisher wheel_encoder_direction("wheel_encoder_direction", &direction_msg);
*/
std_msgs::Int8 click_msg;
ros::Publisher wheel_encoder_clicks("wheel_encoder_clicks", &click_msg);

// callback function for Ros Subscriber

void input_cb ( const std_msgs::Int8& clicks ) {
        target_position += clicks.data;
}

ros::Subscriber<std_msgs::Int8> sub("modify_clicks", &input_cb );


void count_encoder() {
    flag_0 = digitalRead(motor_pins.cnt0);
    flag_1 = digitalRead(motor_pins.cnt1);

    if ( flag_0 == flag_1 ) {
        motor_cnt.cnt0++;
     //   click_msg.data = 1;
    } else {
        motor_cnt.cnt0--;
      //  click_msg.data = 0;
    }

    /*
    direction_msg.data = motor_pins.cnt0;
    wheel_encoder_clicks.publish(&click_msg);
    wheel_encoder_direction.publish(&direction_msg);
    nh.spinOnce();
    */

}

void setup()
{
    nh.initNode();
    nh.subscribe(sub);
    /*
    nh.advertise(wheel_encoder_clicks);
    nh.advertise(wheel_encoder_direction);
    */

    nh.getHardware()->setBaud(57600);
    nh.advertise(wheel_encoder_clicks);
    //Serial.println("Scara-Bot ready");
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

    int test = 8;
    click_msg.data = test;
    wheel_encoder_clicks.publish( &click_msg );

    nh.spinOnce();
    delay(1);
        /*
    monitor = Serial.read();
    Serial.print("Target: ");
    Serial.println(target_position);
    Serial.print("Current: ");
    Serial.println(motor_cnt.cnt0);
    Serial.print("Speed: ");
    Serial.println(positionSpeed);
    Serial.println("-------------");

    if( monitor == 49 ) { // aus
        current_position = 0;
        target_position = 0;
        digitalWrite(motor_pins.left, 0);
        digitalWrite(motor_pins.right, 0);
    }
    if ( monitor == 121 ) {
        target_position += clicks;
    }
    if ( monitor == 120 ) {
        target_position -= clicks;
    }
    if ( monitor == 107 ) {
        clicks-=5;
        target_position -= clicks;
        Serial.print ("Anzahl Schritte: ");
        Serial.println( clicks );
    }
    if ( monitor == 108 ) {
        clicks+=5;
        target_position += clicks;
        Serial.print ("Anzahl Schritte: ");
        Serial.println( clicks );
    }

    _delay_ms(500);
    */
}
