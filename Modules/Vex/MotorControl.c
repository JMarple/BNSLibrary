// MotorControl.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 4/2/2015
//
// This file gives source code for controlling
//  VEX motors with some form of feedback, whether
//  that be IME's, pots, or encoders
//
// Source Code:
//   MotorControl.c
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

#ifndef __BNS_MOTOR_CONTROL_C
#define __BNS_MOTOR_CONTROL_C

#pragma systemFile

#ifndef __BNS_MOTOR_CONTROL_H
#include "MotorControl.h"
#endif

void MotorControlInit(MotorControl* controller, tMotor motor0, tMotor motor1, tMotor motor2, tMotor motor3, tSensors sensor, byte sensorType)
{
	controller->motors[0] = motor0;
	controller->motors[1] = motor1;
	controller->motors[2] = motor2;
	controller->motors[3] = motor3;

	controller->sensorType = sensorType;
	controller->sensor = sensor;
	controller->motorMode = MODE_PWM;

	controller->motorPWM = 0;
	controller->motorPWMLimit = 127;
	controller->targetPosition = 0;
	controller->targetVelocity = 0;

	if(sensorType == SENSOR_IME)
	{
		nMotorEncoder[motor0] = 0;
	}

	RobotDataInit(&controller->sensorData, 5);
}

void MotorControlSetPositionPID(MotorControl* controller, float kP, float kI, float kD)
{
	PIDInit(controller->posPID, kP, kI, kD);
}

void MotorControlSetVelocityPID(MotorControl* controller, float kP, float kI, float kD)
{
	PIDInit(controller->velPID, kP, kI, kD);
}

void MotorControlSetPWM(MotorControl* controller, float target)
{
	controller->motorMode = MODE_PWM;
	controller->motorPWM = target;
}

void MotorControlSetVelocity(MotorControl* controller, float target)
{
	controller->motorMode = MODE_VELOCITY;
	controller->targetVelocity = target;
}

void MotorControlSetPosition(MotorControl* controller, float target)
{
	controller->motorMode = MODE_POSITION;
	controller->targetPosition = target;
}

void MotorControlSetPWMLimit(MotorControl* controller, int limit)
{
	controller->motorPWMLimit = abs(limit);
}

void MotorControlResetPWMLimit(MotorControl* controller)
{
	controller->motorPWMLimit = 127;
}

void _MotorControlLimitPWM(MotorControl* controller)
{
	// Limit PWM
  if(controller->motorPWM > controller->motorPWMLimit)
		controller->motorPWM = controller->motorPWMLimit;
  else if(controller->motorPWM < -controller->motorPWMLimit)
  	controller->motorPWM = -controller->motorPWMLimit;
}

// This updates the actual motor PWM values
void MotorControlUpdate(MotorControl* controller)
{
	// Update Sensor Position, Velocity, and Acceleration information for easily calculating
	//  Position and Velocity PID
	if(controller->sensorType == SENSOR_IME)
	{
			RobotDataUpdatePosition(&controller->sensorData, nMotorEncoder[controller->motors[0]]);
	}
	else if(controller->sensorType == SENSOR_POT)
	{
			RobotDataUpdatePosition(&controller->sensorData, SensorValue[controller->sensor]);
	}
	else if(controller->sensorType == SENSOR_ENCODER)
	{
			// Untested, should work though
			RobotDataUpdatePosition(&controller->sensorData, SensorValue[controller->sensor]);
	}

	// Update Motor PWM depending on which mode we're using
	switch(controller->motorMode)
	{
		// PWM MODE (directly control PWM of the motors)
		case MODE_PWM:

			_MotorControlLimitPWM(controller);

	    // Update all the motors to this PWM
		  int i;
		  for(i = 0; i < MAX_MOTORS; i++)
				if(controller->motors[i] != -1)
					motor[controller->motors[i]] = controller->motorPWM;
		  break;

		// VELOCITY MODE (try to maintain a certain velocity)
		case MODE_VELOCITY:

			// Calculate Velocity PID
			controller->motorPWM +=
							PIDCompute(controller->velPID,
												controller->targetVelocity - RobotDataGetVelocitySummed(&controller->sensorData));

			// Force PWM within bounds of -motorPWMLimits and motorPWMLimts
			_MotorControlLimitPWM(controller);

			// Update all the motors to this PWM
			for(i = 0; i < MAX_MOTORS; i++)
				if(controller->motors[i] != -1)
					motor[controller->motors[i]] = controller->motorPWM;

			break;

		// POSITION MODE (try to maintain a certain position)
		case MODE_POSITION:
		  controller->motorPWM =
		  				PIDCompute(controller->posPID,
		  									controller->targetPosition - controller->sensorData.position);

		  // Force PWM within bounds of -motorPWMLimits and motorPWMLimts
		  _MotorControlLimitPWM(controller);

		 	// Update all the motors to this PWM
			for(i = 0; i < MAX_MOTORS; i++)
				if(controller->motors[i] != -1)
					motor[controller->motors[i]] = controller->motorPWM;

		 	break;
	}
}

#endif
