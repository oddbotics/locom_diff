#include "commands.h"

void sendCommand(char command, char commandArgument = 0) {
  // Write SYNC command, which is necessary before every command
  Serial1.write((byte)SYNC);
  delay(10);
  Serial1.write(command);
  delay(10);
  if (commandArgument) {
    Serial1.write(commandArgument);
  }
}

// Set the mode of the motor driver
void setMode(int mode) {
  currentMode = mode;
  sendCommand(SET_MODE, currentMode);
}

// Gets the speed of the motor
int getSpeed(int motor) {
  if (motor == 1) {
    sendCommand(GET_SPEED_1);
  } else {
    sendCommand(GET_SPEED_2);
  }
  return readSerial1();
}

// Sets the speed of motor 1
void setSpeed1(int spd) {
  // Put the motor driver into a mode that allows individual motor control
  if (currentMode != INDIVIDUAL_MOTOR_CONTROL) {
    setMode(INDIVIDUAL_MOTOR_CONTROL);
  }
  sendCommand(SET_SPEED_1, spd);
}

// Sets the speed of motor 2
void setSpeed2(int spd) {
  // Put the motor driver into a mode that allows individual motor control
  if (currentMode != INDIVIDUAL_MOTOR_CONTROL) {
    setMode(INDIVIDUAL_MOTOR_CONTROL);
  }
  sendCommand(SET_SPEED_2, spd);
}

// Sets the speed of both motors. Don't use this
void setSpeedBoth(int spd) {
  if (currentMode != COMBINED_MOTOR_CONTROL) {
    setMode(COMBINED_MOTOR_CONTROL);
  }
  sendCommand(SET_SPEED_1, spd); 
}

void turn(int turnAmount) {
//  map(turnAmount, 0, 255, -128, 127);
//  PID_motor1_target += turnAmount;
//  PID_motor2_target -= turnAmount;
sendCommand(TURN, turnAmount);

}

// Return speed converted to PID limits
int convertSpeed(int inspeed) {
  int val = inspeed+128;
  if(val > 255){val = 255;}
  if(val < 0){val = 0;}
  return val;
}
