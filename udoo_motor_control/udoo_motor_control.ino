#include "udoo_motor_control.h"
#include "ros.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Int32.h"
#include <std_msgs/Float64.h>

ros::NodeHandle nh;
int sentSpeed;
float rec;
int spd;
int turnAmount;
long timecheck = 0;

long encoder1;
long encoder2;
long encoder1target;
long encoder2target;
int distanceForward = 0;
int degreesTurn = 0;


float mpe = 0.8722;
float turnRadiusConstant = 1;

geometry_msgs::Twist reported_velocity;
ros::Publisher velocityReporter("velocity_reporter", &reported_velocity);

void modeMessageHandler(const std_msgs::Int32& locomotion_mode) {
  current_mode = locomotion_mode.data;
}

ros::Subscriber<std_msgs::Int32> modeSubscription("locomotion_mode", modeMessageHandler);


void velocityMessageHandler(const geometry_msgs::Twist& cmd_vel) {

  //check if cmd_vel is sending velocities
  if (cmd_vel.linear.x != 128)
  {
    spd = cmd_vel.linear.x;
    turnAmount = 128;
    //only go for one second
    //setSpeedBoth(spd);
    timecheck = millis();
  }
  //if there is no forward velocity, check for turning
  if (cmd_vel.angular.z != 128)
  {
    //do some kind of turn
    spd = 128;
    turnAmount = cmd_vel.angular.z;
    timecheck = millis();
  }lo
  if (cmd_vel.angular.z == 128 && cmd_vel.linear.x == 128)
  {
    spd = 128;
    turnAmount = 128;
  }



}

ros::Subscriber<geometry_msgs::Twist> velocitySubscription("cmd_vel_translated", velocityMessageHandler);



void distMessageHandler(const geometry_msgs::Twist& cmd_dist) {

  //distanceForward = cmd_dist.linear.x;
  //degreesTurn = cmd_dist.angular.z;
  encoder1target = cmd_dist.linear.x;
  encoder2target = cmd_dist.linear.y;

}

ros::Subscriber<geometry_msgs::Twist> distSubscription("cmd_dist", distMessageHandler);

geometry_msgs::Twist reported_dist;
ros::Publisher distReporter("dist_reporter", &reported_dist);

void setup() {
  // put your setup code here, to run once:
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);
  // Begin serial and serial 1, which is how we are talking to the motor driver
  Serial1.begin(9600);

  // turn the PID on
  enc1PID.SetMode(AUTOMATIC);
  enc1PID.SetOutputLimits(PID_output_lower, PID_output_upper);
  enc1PID.SetSampleTime(20);

  enc2PID.SetMode(AUTOMATIC);
  enc2PID.SetOutputLimits(PID_output_lower, PID_output_upper);
  enc2PID.SetSampleTime(20);

  // ros stuff
  nh.initNode();
  nh.advertise(velocityReporter);
  nh.subscribe(velocitySubscription);

  nh.advertise(distReporter);
  nh.subscribe(distSubscription);
  
  nh.subscribe(modeSubscription);
  // Set baud rate to handle Twist messages
  nh.getHardware()->setBaud(115200);
  PID1_input = 0;
  PID1_output = 0;
  PID1_target = 0;

  PID2_input = 0;
  PID2_output = 0;
  PID2_target = 0;
  resetEncoders();
  spd = 128;
  turnAmount = 128;
  setSpeedBoth(spd);
  delay(1000);
}


void loop() {
  if (current_mode == VEL_MODE)
  {
    setSpeedBoth(spd);
    
    
    Turn(turnAmount);
  }
  else if (current_mode == DIST_MODE) {

    reported_dist. linear.z = distanceForward;
    encoder1 = getEncoder(1);
    encoder2 = getEncoder(2);

    
      //add the new forward distance to both encoder targets
      //encoder1target = encoder1 + (distanceForward / mpe);
      //encoder2target = encoder2 + (distanceForward / mpe);
      //reset distance forward
      distanceForward = 0;
      PID1_target = encoder1target;
      PID2_target = encoder2target;
    
    if (degreesTurn != 0)
    {
      //find a value to multiply
      encoder1target = encoder1 + (degreesTurn / 2) * turnRadiusConstant;
      encoder2target = encoder2 - (degreesTurn / 2) * turnRadiusConstant;
      degreesTurn = 0;
      PID1_target = encoder1target;
      PID2_target = encoder2target;
    }

    // The target speed is set in the subscriber callback

    // Compute the desired output speed to be sent to the motors
    enc1PID.Compute();
    enc2PID.Compute();
    // get the speed from the encoders
    //spd = getSpeed(1);
    // Set the PID input to the speed gotten from the encoders
    PID1_input = encoder1;
    PID2_input = encoder2;
    //  Serial.println(spd);

    // Set the speed of the motors to the PID controller output
    setSpeed1(PID1_output);
    setSpeed1(PID2_output);
    //  setSpeedBoth(sentSpeed);

    // Report the velocity on the ROS publisher
    //reported_velocity.linear.x = getSpeed(1);


    // Report the dist on the ROS publisher
    // Change to forward distance and total turns?
    reported_dist.linear.x = encoder1 * mpe;
    reported_dist.linear.y = encoder2 * mpe;
    reported_dist.linear.z = distanceForward;
    distReporter.publish(&reported_dist);
  }
  
  reported_velocity.linear.x = getSpeed(1);
  reported_velocity.linear.y = spd;
  reported_velocity.linear.z = turnAmount;
  reported_velocity.angular.x = timecheck;
  reported_velocity.angular.y = current_mode;
  //reported_velocity.linear.x = sentSpeed;
  velocityReporter.publish(&reported_velocity);

  if ((millis() - timecheck) > 1000)
  {
    spd = 128;
    turnAmount = 128;
    setSpeedBoth(spd);

    Turn(turnAmount);
  }
  nh.spinOnce();
  delay(10);
}
