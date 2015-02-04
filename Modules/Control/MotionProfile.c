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
	profile->distance = distance;
}

// This determines the time where, without maxVelocity, the profile changes over
//  from acceleration to deceleration
float MotionProfileDetermineExchangeTime(struct MotionProfile *profile)
{
	float accel = profile->acceleration;
	float decel = -profile->deceleration;

	float veli = profile->startVelocity;
	float vele = profile->endVelocity;

	float distance = profile->distance;

	// Check for valid variables
	if(accel <= 0)
	{
		BNS_ERROR("MOTION PROFILE", "profile->acceleration cannot be <= 0");
		return 0;
	}
	if(decel > 0)
  {
		BNS_ERROR("MOTION PROFILE", "profile->deceleration cannot be <= 0");
		return 0;
	}

	// Perform Quadratic formula
	// Explanation:
	// We need to find the time at which the acceleration and deceleration switch over
	// So we know we will travel s0(distance) when accelerating and s1(distance) when decelerating
	// We will denote the time of the change-over as t
	// From kinematics:
	//  s0 = v_inital * t + 1/2 * acceleration * t^2
	//  s1 = (v_final^2 - v_m^2) / (2*deceleration)
	//  v_m = v_initial + acceleration * t
	//  (where v_m is the velocity at the point of change-over)
	// Since we know the desired distance of travel, s, we can say
	//  s = s0 + s1
	// At this point, do the algebra and you will come out with the following terms for
	//  the quadratic equation when solving for t
	float a = (accel * decel - accel * accel);
	float b = (2 * veli * decel - 2 * veli * accel);
	float c = (vele * vele - veli * veli - distance * 2 * decel);

	float squareTerm = b*b - 4*a*c;
	if(squareTerm < 0)
	{
		BNS_ERROR("MOTION PROFILE", "NEGATIVE SQUARE ROOT ERROR");
    return -1;
  }

  // Choose the option that comes out positive
  float option1 = (-b + sqrt(squareTerm))/(2*a);
  float option2 = (-b - sqrt(squareTerm))/(2*a);

  if(option1 > 0 && option2 < 0)
  	return option1;
  if(option2 > 0 && option1 < 0)
  	return option2;
  if(option1 < 0 && option2 < 0)
  {
  	BNS_ERROR("MOTION PROFILE", "BOTH TIME RESULTS ARE NEGATIVE");
  	return 0;
  }

  return option1;
}

// Determines if the motion profile is complete
// This is a copy/paste of the MotionProfileCompute function, just return functions
// return true/false.  Probably could be optimized at some point.
bool MotionProfileIsComplete(struct MotionProfile *profile, float time)
{
	// Current velocity(if max velocity didn't exist)
	float vel1 = profile->startVelocity + profile->acceleration * time;

	// Determine at best case, when we would change from acceleration to deceleration
	float timeToExchange = MotionProfileDetermineExchangeTime(profile);
	float velocityAtExchange = profile->startVelocity + profile->acceleration * timeToExchange;

	// Check to see if this profile will reach max velocity or not
	if(velocityAtExchange < profile->maxVelocity)
	{
		// Check if the profile is still accelerating
		if(timeToExchange > time)
			return false;

		float timeToDecelerate = (abs(profile->endVelocity - velocityAtExchange)) / profile->deceleration;

		if(time > timeToDecelerate + timeToExchange)
			return true;
	  else
			return false;
	}
	else
	{
		// How long does it take to accelerate to max speed
  	float timeToMaxVel = (profile->maxVelocity - profile->startVelocity) / (profile->acceleration);

		// Calculate distance at current velocity
		float distanceToStop = (vel1 * vel1 - profile->endVelocity * profile->endVelocity) / (2.0 * profile->deceleration);
		float distanceSoFar = profile->startVelocity * time + 0.5 * profile->acceleration*time*time;

		// If the profile reached max speed
		if(timeToMaxVel < time)
		{
			// Calculate distance at maxVelocity
			float distanceFromAcceleration = profile->startVelocity * timeToMaxVel + 0.5 * profile->acceleration*timeToMaxVel*timeToMaxVel;
		  float distanceFromMaxVel = profile->maxVelocity * (time - timeToMaxVel);
			float distanceSoFar = distanceFromAcceleration + distanceFromMaxVel;
			float distanceToStop = (profile->maxVelocity * profile->maxVelocity - profile->endVelocity * profile->endVelocity) / (2.0 * profile->deceleration);

			// Calculate how far we stay at max velocity
			float distanceAtMaxVelocity = profile->distance - distanceFromAcceleration - distanceToStop;
			float timeWithMax = distanceAtMaxVelocity / profile->maxVelocity;

			if(distanceSoFar + distanceToStop < profile->distance)
			{
				return false;
			}
		  else
		  {
		  	// Time offset to the point we started to decelerate
		  	float decelTime = time - timeWithMax - timeToMaxVel;
		  	float timeToDecelerate = abs((profile->endVelocity - profile->maxVelocity) / (profile->deceleration));

		  	if(time > timeToMaxVel + timeWithMax + timeToDecelerate)
		  		return true;
			  else
					return false;
			}
		}
		else
		{
			return false;
		}
	}
	return false;
}

// Computes the appropriate velocity at a certain point of time
float MotionProfileCompute(struct MotionProfile *profile, float time)
{
	// Current velocity(if max velocity didn't exist)
	float vel1 = profile->startVelocity + profile->acceleration * time;

	// Determine at best case, when we would change from acceleration to deceleration
	float timeToExchange = MotionProfileDetermineExchangeTime(profile);
	float velocityAtExchange = profile->startVelocity + profile->acceleration * timeToExchange;

	// Check to see if this profile will reach max velocity or not
	if(velocityAtExchange < profile->maxVelocity)
	{
		// Check if the profile is still accelerating
		if(timeToExchange > time)
			return vel1;

		float timeToDecelerate = (abs(profile->endVelocity - velocityAtExchange)) / profile->deceleration;

		if(time > timeToDecelerate + timeToExchange)
			return profile->endVelocity;
	  else
			return velocityAtExchange - profile->deceleration * (time - timeToExchange);
	}
	else
	{
		// How long does it take to accelerate to max speed
  	float timeToMaxVel = (profile->maxVelocity - profile->startVelocity) / (profile->acceleration);

		// Calculate distance at current velocity
		float distanceToStop = (vel1 * vel1 - profile->endVelocity * profile->endVelocity) / (2.0 * profile->deceleration);
		float distanceSoFar = profile->startVelocity * time + 0.5 * profile->acceleration*time*time;

		// If the profile reached max speed
		if(timeToMaxVel < time)
		{
			// Calculate distance at maxVelocity
			float distanceFromAcceleration = profile->startVelocity * timeToMaxVel + 0.5 * profile->acceleration*timeToMaxVel*timeToMaxVel;
		  float distanceFromMaxVel = profile->maxVelocity * (time - timeToMaxVel);
			float distanceSoFar = distanceFromAcceleration + distanceFromMaxVel;
			float distanceToStop = (profile->maxVelocity * profile->maxVelocity - profile->endVelocity * profile->endVelocity) / (2.0 * profile->deceleration);

			// Calculate how far we stay at max velocity
			float distanceAtMaxVelocity = profile->distance - distanceFromAcceleration - distanceToStop;
			float timeWithMax = distanceAtMaxVelocity / profile->maxVelocity;

			if(distanceSoFar + distanceToStop < profile->distance)
			{
				return profile->maxVelocity;
			}
		  else
		  {
		  	// Time offset to the point we started to decelerate
		  	float decelTime = time - timeWithMax - timeToMaxVel;
		  	float timeToDecelerate = abs((profile->endVelocity - profile->maxVelocity) / (profile->deceleration));

		  	if(time > timeToMaxVel + timeWithMax + timeToDecelerate)
		  		return profile->endVelocity;
			  else
					return profile->maxVelocity - profile->deceleration * decelTime;
			}
		}
		else
		{
			return vel1;
		}
	}

	// This should never get to this point...
	BNS_ERROR("MOTION_PROFILE", "END OF COMPUTE ERROR, RETURNING -1");

	return -1;
}

#endif
