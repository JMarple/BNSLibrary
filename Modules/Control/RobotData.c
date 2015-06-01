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
void RobotDataInit(RobotData* rData, int size)
{
	rData->cirBufferSize = size;
	CircularBufferInit(&rData->position, float, size);
	CircularBufferInit(&rData->velocity, float, size);
	CircularBufferInit(&rData->acceleration, float, size);

	float initialValue = 0;

	for(int i = 0; i < size; i++)
	{
		CircularBufferAdd(&rData->position, &initialValue);
		CircularBufferAdd(&rData->velocity, &initialValue);
		CircularBufferAdd(&rData->acceleration, &initialValue);
	}
}

// Updates the position, and then calculates the corresponding velocities
// and acceleration from that
void RobotDataUpdatePosition(RobotData* rData, float data)
{
	float prevPosition = RobotDataGetPosition(rData, 0);
	float prevVelocity = RobotDataGetPosition(rData, 0);
	float position = data;
	float velocity = prevPosition - position;
	float acceleration = prevVelocity - velocity;

	CircularBufferGet(&rData->position);
	CircularBufferGet(&rData->velocity);
	CircularBufferGet(&rData->acceleration);

	CircularBufferAdd(&rData->position, &position);
	CircularBufferAdd(&rData->velocity, &velocity);
	CircularBufferAdd(&rData->acceleration, &acceleration);
}

// Updates the velocity, and then integrates position and calculates
//  the corresponding acceleration
void RobotDataUpdateVelocity(RobotData* rData, float velocity)
{
  float position = RobotDataGetPosition(rData, 0) + velocity;
  float acceleration = RobotDataGetPosition(rData, 0) - velocity;

	CircularBufferGet(&rData->position);
	CircularBufferGet(&rData->velocity);
	CircularBufferGet(&rData->acceleration);

	CircularBufferAdd(&rData->position, &position);
	CircularBufferAdd(&rData->velocity, &velocity);
	CircularBufferAdd(&rData->acceleration, &acceleration);
}

// Gets the position from the circular buffer.  Note: where = 0 is the most recent data
float RobotDataGetPosition(RobotData* rData, int where)
{
	float* x = (float*)CircularBufferPeek(&rData->position, rData->cirBufferSize - 1 - where);
	return *x;
}

// Gets the velocity from the circular buffer.  Note: where = 0 is the most recent data
float RobotDataGetVelocity(RobotData* rData, int where)
{
	float* x = (float*)CircularBufferPeek(&rData->velocity, rData->cirBufferSize - 1 - where);
	return *x;
}

// Gets the acceleration from the circular buffer.  Note: where = 0 is the most recent data
float RobotDataGetAcceleration(RobotData* rData, int where)
{
	float* x = (float*)CircularBufferPeek(&rData->acceleration, rData->cirBufferSize - 1 - where);
	return *x;
}

// Sums all the positional data together
float RobotDataGetPositionSummed(RobotData* rData)
{
	float value = 0;
	for(int i = 0; i < rData->cirBufferSize; i++)
	{
		value += RobotDataGetPosition(rData, i);
	}

	return value;
}

// Sums all the velocity data together
float RobotDataGetVelocitySummed(RobotData* rData)
{
	float value = 0;
	for(int i = 0; i < rData->cirBufferSize; i++)
	{
		value += RobotDataGetVelocity(rData, i);
	}

	return value;
}

// Sums all the acceleration data together
float RobotDataGetAccelerationSummed(RobotData* rData)
{
	float value = 0;
	for(int i = 0; i < rData->cirBufferSize; i++)
	{
		value += RobotDataGetAcceleration(rData, i);
	}

	return value;
}
#endif
