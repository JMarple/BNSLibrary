#pragma systemFile

#ifndef __BNS_MOTION_PROFILE_COMPUTATION_C
#define __BNS_MOTION_PROFILE_COMPUTATION_C

#ifndef __BNS_MOTION_PROFILE_H
#include "MotionProfile.h"
#endif

// This determines the time where, without maxVelocity, the profile changes over
//  from acceleration to deceleration
float MotionProfileDetermineExchangeTime(struct MotionProfile *profile)
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

// Forces all velocities to be positive
float MotionProfileCreateOffset(struct MotionProfile *profile)
{
	float smallest = profile->startVelocity;
	if(profile->maxVelocity < smallest)
		smallest = profile->maxVelocity;
	if(profile->endVelocity < smallest)
		smallest = profile->endVelocity;

	return smallest;
}

float MotionProfileGetVelocityWithoutMaxVelocity(struct MotionProfile *profile, float time)
{
	// Are we accelerating forward or backwards?
  // float dir = 1 - 2*(profile->startVelocity > profile->maxVelocity);

	// Current velocity(if max velocity didn't exist)
	float vel1 = profile->startVelocity + profile->acceleration * time;

	// Determine at best case, when we would change from acceleration to deceleration
	float timeToExchange = MotionProfileDetermineExchangeTime(profile);
	float velocityAtExchange = profile->startVelocity + profile->acceleration * timeToExchange;

	// Report possible errors
	if(timeToExchange == BNS_ERROR_CODE)
		return BNS_ERROR_CODE;

  //writeDebugStreamLine("timeToExchange = %f", timeToExchange);
	//writeDebugStreamLine("velocityAtExchange = %f", velocityAtExchange);

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
			returnVelocity = profile->endVelocity;
	  else
			returnVelocity = velocityAtExchange + profile->deceleration * (time - timeToExchange);
	}

	return returnVelocity;
}

float MotionProfileGetVelocityWithMaxVelocity(struct MotionProfile *profile, float time)
{
	// Current velocity(if max velocity didn't exist)
	float vel1 = profile->startVelocity + profile->acceleration * time;

	// How long does it take to accelerate to max speed
	float timeToMaxVel = (profile->maxVelocity - profile->startVelocity) / (profile->acceleration);

	// Calculate distance at current velocity
	//float distanceToStop = (vel1 * vel1 - profile->endVelocity * profile->endVelocity) / (2.0 * profile->deceleration);
	float distanceSoFar = profile->startVelocity * time + 0.5 * profile->acceleration*time*time;

	// Calculate distance at maxVelocity
	float distanceFromAcceleration = profile->startVelocity * timeToMaxVel + 0.5 * profile->acceleration*timeToMaxVel*timeToMaxVel;
  float distanceFromMaxVel = profile->maxVelocity * (time - timeToMaxVel);
	float distanceSoFarWithMax = distanceFromAcceleration + distanceFromMaxVel;

	float timeToStop = (profile->endVelocity - profile->maxVelocity) / (profile->deceleration);
	float distanceToStop = profile->maxVelocity*timeToStop + 0.5 * profile->deceleration * timeToStop * timeToStop; //(profile->maxVelocity * profile->maxVelocity - profile->endVelocity * profile->endVelocity) / (2.0 * profile->deceleration);

	// Calculate how far we stay at max velocity
	float distanceAtMaxVelocity = profile->distance - distanceFromAcceleration - distanceToStop;
	float timeWithMax = distanceAtMaxVelocity / profile->maxVelocity;

	// Report possible errors
	float timeToExchange = MotionProfileDetermineExchangeTime(profile);
	if(timeToExchange == BNS_ERROR_CODE)
		return BNS_ERROR_CODE;

	// If the profile reached max speed
	if(timeToMaxVel < time)
	{
		//writeDebugStreamLine("distanceSoFarWIthMax = %f", distanceSoFarWithMax);
		//writeDebugStreamLine("distanceToStopAtMax = %f", distanceToStop);

		if(distanceSoFarWithMax + distanceToStop < profile->distance)
		{
			return profile->maxVelocity;
		}
	  else
	  {
	  	// Time offset to the point we started to decelerate
	  	float decelTime = time - timeWithMax - timeToMaxVel;

	  	if(time > timeToMaxVel + timeWithMax + timeToStop)
	  		return profile->endVelocity;
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
