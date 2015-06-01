// MotorControl.h
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 4/2/2015
//
// This file outlines the code for controlling
//  VEX motors with some form of feedback, whether
//  that be IME's, pots, or encoders
//
// Source Code:
//   MotorControl.h
//
// Dependencies:
//    None!
//
// ------------------------------------------------------------------------
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// The author can be contacted by email at jmarple@umass.edu
//
// ------------------------------------------------------------------------

#ifndef __BNS_MOTOR_CONTROL_H
#define __BNS_MOTOR_CONTROL_H

#ifndef __BNS_ROBOT_DATA_H
#include "../Control/RobotData.h"
#endif

#ifndef __BNS_PID_CONTROLLER_H
#include "../Control/PIDController.h"
#endif

#define SENSOR_IME      0x01
#define SENSOR_ENCODER  0x02
#define SENSOR_POT      0x04

#define MODE_PWM				0x01
#define MODE_VELOCITY		0x02
#define MODE_POSITION		0x03

#define MAX_MOTORS 4

struct MotorControl
{
	// Feedback Data for this motor
	struct RobotData sensorData;

	struct PID posPID;
	struct PID velPID;

	// Motors being controlled (set extra slots to -1)
	tMotor motors[MAX_MOTORS];
	int motorPWM;
	int motorPWMLimit;

	// Target Position/Velocity
	float targetPosition;
	float targetVelocity;

	// Which type of sensor are we using for feedback?
	// SENSOR_IME, SENSOR_ENCODER, SENSOR_POT?
	byte sensorType;

	// The sensor we are using
	tSensors sensor;

	// Which mode are we running this controller in?
	// MODE_PWM, MODE_VELOCITY, MODE_POSITION?
	byte motorMode;
};

void MotorControlInit(MotorControl* controller, tMotor motor0, tMotor motor1, tMotor motor2, tMotor motor3, tSensors sensor, byte sensorType);
void MotorControlSetPositionPID(MotorControl* controller, float kP, float kI, float kD);
void MotorControlSetVelocityPID(MotorControl* controller, float kP, float kI, float kD);
void MotorControlSetPWM(MotorControl* controller, float target);
void MotorControlSetVelocity(MotorControl* controller, float target);
void MotorControlSetPosition(MotorControl* controller, float target);
void MotorControlSetPWMLimit(MotorControl* controller, int limit);
void MotorControlResetPWMLimit(MotorControl* controller);
void MotorControlUpdate(MotorControl* controller);

#endif
