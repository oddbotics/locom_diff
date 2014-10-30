#include "udoo_motor_control.h"



void setup() {
  // put your setup code here, to run once:
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);
  // Begin serial and serial 1, which is how we are talking to the motor driver
  Serial1.begin(9600);
  Serial.begin(9600);

  // turn the PID on
  speedPID.SetMode(AUTOMATIC);
  speedPID.SetOutputLimits(0, 255);
  speedPID.SetSampleTime(20);
}


void loop() {

  speedPID.Compute();
  int spd = getSpeed(1);
  PID_input = spd;
  //  Serial.print("Speed: ");
  Serial.println(spd);
  //  Serial.print("PID Output: ");
  //  Serial.println((int)PID_output);
  //  Serial.print("PID Target: ");
  //  Serial.println((int)PID_target);
  //    Serial.println();
  setSpeedBoth(PID_output);

}



