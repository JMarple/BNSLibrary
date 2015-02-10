// MotionProfileComputation.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 2/10/2015
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

#ifndef __BNS_MOTION_PROFILE_COMPUTATION_C
#define __BNS_MOTION_PROFILE_COMPUTATION_C

#ifndef __BNS_MOTION_PROFILE_H
#include "MotionProfile.h"
#endif

// Checks that the input is runable
float _MotionProfileCheckParameters(struct MotionProfile* profile)
{
	float dirstart = 1 - 2*(profile->startVelocity > profile->maxVelocity);
	float dirend = 1 - 2*(profile->endVelocity > profile->maxVelocity);

	if(profile->acceleration == 0 && profile->startVelocity != profile->endVelocity)
	{
		BNS_ERROR("MOTION PROFILE ERROR", "ACCELERATION IS ZERO, CHANGED TO 1");
		profile->acceleration = 1*dirstart;
	}

	if(profile->deceleration == 0 && profile->maxVelocity != profile->endVelocity)
	{
		BNS_ERROR("MOTION PROFILE ERROR", "DECELERATION IS ZERO, CHANGED TO 1");
		profile->deceleration = 1*-dirstart;
	}

	if((profile->deceleration > 0 && profile->acceleration > 0)
		|| (profile->deceleration < 0 && profile->acceleration < 0))
	{
		BNS_ERROR("MOTION PROFILE ERROR", "ACCELERATION/DECELERATION CANNOT BE THE SAME, FLIPPED DECELERATION");
		profile->deceleration = -profile->deceleration;
	}

	if( (dirstart < 0 && profile->acceleration > 0)||(dirstart > 0 && profile->acceleration < 0) )
	{
		BNS_ERROR("MOTION PROFILE ERROR", "ACCELERATION SIGN DOES NOT MATCH START/MAX VELOCITY, FLIP NEGATIVE SIGN");
	}

	if( (dirend > 0 && profile->deceleration > 0)||(dirend < 0 && profile->deceleration < 0))
	{
		BNS_ERROR("MOTION PROFILE ERROR", "DECELERATION SIGN DOES NOT MATCH MAX/END VELOCITY, FLIP NEGATIVE SIGN");
	}

	return 0;
}

// This determines the time where, without maxVelocity, the profile changes over
//  from acceleration to deceleration
float _MotionProfileDetermineExchangeTime(struct MotionProfile *profile)
{
	float accel = profile->acceleration;
	float decel = profile->deceleration;

	float veli = profile->startVelocity;
	float vele = profile->endVelocity;

	float distance = profile->distance;

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
		BNS_ERROR("MOTION PROFILE ERROR", "IMPOSSIBLE TO REACH LOCATION");
		return BNS_ERROR_CODE;
	}

	// Acceleration must equal zero, so assume max velocity will
	//  be immediately reached
	if(a == 0)
	{
		return 0;
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


float _MotionProfileGetVelocityWithoutMaxVelocity(struct MotionProfile *profile, float time, bool *isComplete)
{
	// Current velocity(if max velocity didn't exist)
	float vel1 = profile->startVelocity + profile->acceleration * time;

	// Determine at best case, when we would change from acceleration to deceleration
	float timeToExchange = _MotionProfileDetermineExchangeTime(profile);
	float velocityAtExchange = profile->startVelocity + profile->acceleration * timeToExchange;

	// Report possible errors
	if(timeToExchange == BNS_ERROR_CODE)
		return BNS_ERROR_CODE;

	double returnVelocity = -1;

	// Still accelerating to velocityAtExchange?
	if(timeToExchange > time)
	{
		returnVelocity = vel1;
	}
	else
	{
		float timeToDecelerate = (profile->endVelocity - velocityAtExchange) / profile->deceleration;

		if(time > timeToDecelerate + timeToExchange)
		{
			*isComplete = true;
			returnVelocity = profile->endVelocity;
		}
		else
			returnVelocity = velocityAtExchange + profile->deceleration * (time - timeToExchange);
	}

	return returnVelocity;
}

float _MotionProfileGetVelocityWithMaxVelocity(struct MotionProfile *profile, float time, bool *isComplete)
{
	// Are we accelerating forward or backwards?
	float dir = 1 - 2*(profile->startVelocity > profile->maxVelocity || profile->endVelocity > profile->maxVelocity);

	// Current velocity(if max velocity didn't exist)
	float vel1 = profile->startVelocity + profile->acceleration * time;

	// How long does it take to accelerate to max speed
	float timeToMaxVel = 0;

	// Check for divide by zero errors
	if(profile->acceleration != 0)
		timeToMaxVel = (profile->maxVelocity - profile->startVelocity) / (profile->acceleration);

	// Calculate acceleration distance, current distance traveled at max velocity and distnace to stop
	float distanceFromAcceleration = profile->startVelocity * timeToMaxVel + 0.5 * profile->acceleration*timeToMaxVel*timeToMaxVel;
	float distanceFromMaxVel = profile->maxVelocity * (time - timeToMaxVel);
	float timeToStop = 0;
	if(profile->deceleration != 0)// Check for divide by zero errors
		timeToStop = (profile->endVelocity - profile->maxVelocity) / (profile->deceleration);

	float distanceToStop = profile->maxVelocity * timeToStop + 0.5 * profile->deceleration * timeToStop * timeToStop;

	// Calculate how far we stay at max velocity
	float distanceAtMaxVelocity = profile->distance - distanceFromAcceleration - distanceToStop;
	float timeWithMax = distanceAtMaxVelocity / profile->maxVelocity;

	// Report possible errors
	float timeToExchange = _MotionProfileDetermineExchangeTime(profile);
	if(timeToExchange == BNS_ERROR_CODE)
		return BNS_ERROR_CODE;

	// If the profile reached max speed
	if(timeToMaxVel < time)
	{
		if((distanceFromAcceleration + distanceFromMaxVel + distanceToStop)*dir < profile->distance*dir)
		{
			return profile->maxVelocity;
		}
		else
		{
			// Time offset to the point we started to decelerate
			float decelTime = time - timeWithMax - timeToMaxVel;

			if(time > timeToMaxVel + timeWithMax + timeToStop)
			{
				*isComplete = true;
				return profile->endVelocity;
			}
			else
				return profile->maxVelocity + profile->deceleration * decelTime;
		}
	}
	else
	{
		// Still accelerating to max velocity
		return vel1;
	}
}

#endif
