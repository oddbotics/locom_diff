#include "udoo_motor_control.h"
#include "ros.h"
#include "geometry_msgs/Twist.h"
#include <std_msgs/Float64.h>

ros::NodeHandle nh;
int sentSpeed;
float rec;
int spd;
int turnAmount = 128;

void velocityMessageHandler(const geometry_msgs::Twist& cmd_vel) {
  PID_motor1_target = cmd_vel.linear.x; // motor 1 target velocity
  turnAmount = cmd_vel.angular.z; //set turn amount from angular z
}

ros::Subscriber<geometry_msgs::Twist> velocitySubscription("cmd_vel", velocityMessageHandler);

geometry_msgs::Twist reported_velocity;
ros::Publisher velocityReporter("velocity_reporter", &reported_velocity);


void setup() {
  // put your setup code here, to run once:
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);
  // Begin serial and serial 1, which is how we are talking to the motor driver
  Serial1.begin(9600);

  // turn the PID for motor 1 on
  motor1PID.SetMode(AUTOMATIC);
  motor1PID.SetOutputLimits(PID_output_lower, PID_output_upper);
  motor1PID.SetSampleTime(20);

  // turn the PID for motor 2 on
  motor2PID.SetMode(AUTOMATIC);
  motor2PID.SetOutputLimits(PID_output_lower, PID_output_upper);
  motor2PID.SetSampleTime(20);

  // ros stuff
  nh.initNode();
  nh.advertise(velocityReporter);
  nh.subscribe(velocitySubscription);

  // Set baud rate to handle Twist messages
  nh.getHardware()->setBaud(115200);
}


void loop() {
  // The target speed is set in the subscriber callback

  // get the speed of the motors
  motor1_speed = getSpeed(1);
  motor2_speed = getSpeed(2);

  // Set the PID input to the speed gotten from the encoders
  PID_motor1_input = motor1_speed;
  PID_motor2_input = motor2_speed;

  // Compute the desired output speed to be sent to the motors
  motor1PID.Compute();
  motor2PID.Compute();

  // Set the speed of the motors to the PID controller output
  setSpeedBoth(PID_motor1_target);
  
  //Set turn
  turn(turnAmount);

  // Set values to be reported by ROS publisher
  reported_velocity.linear.x = motor1_speed;

  // It is pointless to return this, since it means the reported velocity angular z will always be the same as cmd_vel.angular.z
  reported_velocity.angular.z = turnAmount;

  // Publish the reported velocity
  velocityReporter.publish(&reported_velocity);

  // They spinnin
  nh.spinOnce();
  delay(20);
}
