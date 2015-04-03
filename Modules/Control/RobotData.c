// RobotData.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 4/2/2015
//
// This file implements a way to easily keep track of position, velocity
//  and acceleration.  By giving a RobotData struct a given position
//  or velocity in a constant loop, this file will automatically
//  calculate the corresponding positions, velocities and accelerations.
// This is designed to be generally used for sensors inputs (ie, you give
//  the RobotData functions the sensor data, and it will automatically
//  calculate position, velocity and acceration of those sensors from that.
//
// Source Code:
//   RobotData.c
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

#pragma systemFile

#ifndef __BNS_ROBOT_DATA_C
#define __BNS_ROBOT_DATA_C

#ifndef __BNS_ROBOT_DATA_H
#include "RobotData.h"
#endif

// Initializes all data to 0
void RobotDataInit(RobotData* rData)
{
	rData->position = 0;
	rData->velocity = 0;
	rData->acceleration = 0;

	rData->previousPosition = 0;
	rData->previousVelocity = 0;
	rData->previousAcceleration = 0;
}

// Updates the position, and then calculates the corresponding velocities
// and acceleration from that
void RobotDataUpdatePosition(RobotData* rData, float data)
{
	rData->previousPosition = rData->position;
	rData->previousVelocity = rData->velocity;
	rData->previousAcceleration = rData->velocity;

	rData->position = data;
	rData->velocity = rData->previousPosition - rData->position;
	rData->acceleration = rData->previousVelocity - rData->velocity;
}

// Updates the velocity, and then integrates position and calculates
//  the corresponding acceleration
void RobotDataUpdateVelocity(RobotData* rData, float velocity)
{
	rData->previousPosition = rData->position;
	rData->previousVelocity = rData->velocity;
	rData->previousAcceleration = rData->velocity;

	rData->velocity = velocity;
	rData->position += rData->velocity;
	rData->acceleration = rData->previousVelocity - rData->velocity;
}

#endif
