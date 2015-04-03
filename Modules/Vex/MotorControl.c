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

#ifndef __BNS_MOTOR_CONTROL_H
#include "MotorControl.h"
#endif

void MotorControlInit(MotorControl* controller, word motor0, word motor1, word motor2, word motor3, word sensor, byte sensorType)
{
	controller->motors[0] = motor0;
	controller->motors[1] = motor1;
	controller->motors[2] = motor2;
	controller->motors[3] = motor3;

	controller->sensorType = sensorType;
	controller->sensor = sensor;
	controller->motorMode = MODE_PWM;

	controller->motorPWM = 0;
	controller->targetPosition = 0;
	controller->targetVelocity = 0;

	PIDInit(controller->pidContr, 0.2, 0.0, 0.0);
}

void MotorControlSetPWMMode(MotorControl* controller)
{
	controller->motorMode = MODE_PWM;
}

void MotorControlSetVelocityMode(MotorControl* controller)
{
	controller->motorMode = MODE_VELOCITY;
}

void MotorControlSetPositionMode(MotorControl* controller)
{
	controller->motorMode = MODE_POSITION;
}

// Sets a new target value for whichever mode it is in.  For instance,
//  if it is in velocity/position mode, it will set the target position/velocity
//  If it is in PWM mode, it will set the PWM value
void MotorControlSet(MotorControl* controller, float target)
{
	switch(controller->motorMode)
	{
		case MODE_PWM:
			controller->motorPWM = target;
			break;

		case MODE_VELOCITY:
			controller->targetVelocity = target;
			break;

		case MODE_POSITION:
		  controller->targetPosition = target;
		  break;
	}
}

// This updates the actual motor PWM values
void MotorControlUpdate(MotorControl* controller)
{
		switch(controller->motorMode)
		{
			case MODE_PWM:
			  int i;
			  for(i = 0; i < MAX_MOTORS; i++)
					if(controller->motors[i] != -1)
						motor[controller->motors[i]] = controller->motorPWM;
			  break;

			case MODE_VELOCITY:

				controller->motorPWM +=
								PIDCompute(controller->pidContr,
													controller->targetVelocity - controller->sensorData.velocity);


				break;

			case MODE_POSITION:
				break;

		}
}

#endif
