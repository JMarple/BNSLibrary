// MotionProfile.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 2/3/2015
//
// This source file includes source code that
// implements a Motion Controller for use in
// Vex Robotics Competition.
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

#ifndef __BNS_MOTION_PROFILE_C
#define __BNS_MOTION_PROFILE_C

#ifndef __BNS_MOTION_PROFILE_H
#include "MotionProfile.h"
#endif

// Load defaults for motion profile
void MotionProfileInit(struct MotionProfile *profile)
{
	profile->acceleration = 1;
	profile->deceleration = 1;

	profile->startVelocity = 0;
	profile->maxVelocity = 10;
	profile->endVelocity = 0;

	profile->distance = 1;
}

// Set Acceleration for Motion Profile
void MotionProfileSetAccel(struct MotionProfile *profile, float accel, float decel)
{
	profile->acceleration = accel;
	profile->deceleration = decel;
}

// Set Velocity for Motion Profile
void MotionProfileSetVelocity(struct MotionProfile *profile, float enterVel, float maxVel, float exitVel)
{
	profile->startVelocity = enterVel;
	profile->maxVelocity = maxVel;
	profile->endVelocity = exitVel;
}

// Set Distance for Motion Profile
void MotionProfileSetDistance(struct MotionProfile *profile, float distance)
{
	profile->distance = abs(distance);
}

// Forces all velocities to be positive
float MotionProfileCreateOffset(struct MotionProfile *profile)
{
	float smallest = profile->startVelocity;
	if(profile->maxVelocity < smallest)
		smallest = profile->maxVelocity;
	if(profile->endVelocity < smallest)
		smallest = profile->endVelocity;

	profile->startVelocity-=smallest;
	profile->maxVelocity-=smallest;
	profile->endVelocity-=smallest;

	return smallest;
}

// Determines if the motion profile is complete
bool MotionProfileIsComplete(struct MotionProfile *profile, float time)
{
	float offset = MotionProfileCreateOffset(profile);

	// Are we accelerating forward or backwards?
  float dir = 1 - 2*(profile->startVelocity > profile->maxVelocity);

	// Determine at best case, when we would change from acceleration to deceleration
	float timeToExchange = MotionProfileDetermineExchangeTime(profile);
	float velocityAtExchange = profile->startVelocity + profile->acceleration * timeToExchange * dir;

	bool returnValue;

	// If true, this profile will not reach max velocity
	if(velocityAtExchange*dir < profile->maxVelocity*dir)
	{
		float vel = MotionProfileGetVelocityWithoutMaxVelocity(profile, time);
		if(vel == profile->endVelocity && time > timeToExchange)
			returnValue = true;
		else
			returnValue = false;
	}
	else
	{
		float vel = MotionProfileGetVelocityWithMaxVelocity(profile, time);
		if(vel == profile->endVelocity && time > timeToExchange)
			returnValue = true;
		else
			returnValue = false;
	}

	profile->maxVelocity += offset;
	profile->startVelocity += offset;
	profile->endVelocity += offset;

	return returnValue;
}

// Computes the appropriate velocity at a certain point of time
float MotionProfileCompute(struct MotionProfile *profile, float time)
{
	float offset = MotionProfileCreateOffset(profile);

	// Are we accelerating forward or backwards?
  float dir = 1 - 2*(profile->startVelocity > profile->maxVelocity);

	// Determine at best case, when we would change from acceleration to deceleration
	float timeToExchange = MotionProfileDetermineExchangeTime(profile);
	float velocityAtExchange = profile->startVelocity + profile->acceleration * timeToExchange * dir;

	// This is the value the program will return for velocity
	float returnVelocity = -1;

	// If true, this profile will not reach max velocity
	if(velocityAtExchange*dir < profile->maxVelocity*dir)
	{
		returnVelocity = MotionProfileGetVelocityWithoutMaxVelocity(profile, time);
	}
	else
	{
		returnVelocity = MotionProfileGetVelocityWithMaxVelocity(profile, time);
	}

	profile->maxVelocity += offset;
	profile->startVelocity += offset;
	profile->endVelocity += offset;

	return returnVelocity + offset;
}

// Prints what the motion profile will execute.  Set dt to what your time scale is
//  going to be (for instance, if you are excuting your profile at 10 times per second,
//  set dt to 0.1.  Be sure dt are the same units as you set in your motion profile!
void MotionProfilePrint(struct MotionProfile* profile, float dt)
{
	// "Execute" motion profile
  float time = 0;
  while(!MotionProfileIsComplete(profile, time))
	{
  	float velocity = MotionProfileCompute(profile, time);
  	writeDebugStreamLine("%f", velocity);
		time+=dt;
		delay(2);
  }
}

#endif
