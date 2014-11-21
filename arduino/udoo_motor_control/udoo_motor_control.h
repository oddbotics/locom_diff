#ifndef UDOO_MOTOR_CONTROL_H_
#define UDOO_MOTOR_CONTROL_H_

#include "PID_v1.h"

#define RX_PIN 158 // Yellow, goes to TX on Udoo
#define TX_PIN 159 // Orange, goes to RX on Udoo
#define WHEEL_DIAMETER 100 // 100mm
#define SERIAL_BUFFER_LEN 32

int PID_output_lower = 0;
int PID_output_upper = 255;

double PID_motor1_input = 128;
double PID_motor1_output = 128;
double PID_motor1_target = 128;

double PID_motor2_input = 128;
double PID_motor2_output = 128;
double PID_motor2_target = 128;

double Kp = 0.1;
double Ki = 2.0;
double Kd = 0.01;

PID motor1PID(&PID_motor1_input, &PID_motor1_output, &PID_motor1_target, Kp, Ki, Kd, DIRECT);
PID motor2PID(&PID_motor2_input, &PID_motor2_output, &PID_motor2_target, Kp, Ki, Kd, DIRECT);

int motor1_speed;
int motor2_speed;

int currentMode;

// Contains the serial output
char serial1_buffer[SERIAL_BUFFER_LEN];
int serial1_buffer_position = 0;

#endif
