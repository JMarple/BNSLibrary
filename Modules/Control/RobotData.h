// RobotData.h
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

#ifndef __BNS_ROBOT_DATA_H
#define __BNS_ROBOT_DATA_H

struct RobotData
{
	float position;
	float velocity;
	float acceleration;

	float previousPosition;
	float previousVelocity;
	float previousAcceleration;
};

// RobotDataInit(...)
// Initializes all the variables in the struct to zeo
void RobotDataInit(RobotData* rData);

// RobotDataUpdatePosition(...)
// Given a position input, this function will updates the
//  velocity and acceleration variables
void RobotDataUpdatePosition(RobotData* rData, float data);

// RobotDataUpdateVelocity(...)
// Given a velocity input, this function will update the
//  position and acceleration variables
void RobotDataUpdateVelocity(RobotData* rData, float velocity);

#endif
