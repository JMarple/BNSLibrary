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

float MotionProfileGetVelocityWithoutMaxVelocity(struct MotionProfile *profile, float time)
{
	// Are we accelerating forward or backwards?
  float dir = 1 - 2*(profile->startVelocity > profile->maxVelocity);

	// Current velocity(if max velocity didn't exist)
	float vel1 = profile->startVelocity + profile->acceleration * time * dir;

	// Determine at best case, when we would change from acceleration to deceleration
	float timeToExchange = MotionProfileDetermineExchangeTime(profile);
	float velocityAtExchange = profile->startVelocity + profile->acceleration * timeToExchange * dir;

	// Still accelerating to velocityAtExchange?
	if(timeToExchange > time)
	{
		return vel1;
	}
	else
	{
		float timeToDecelerate = (profile->endVelocity - velocityAtExchange) / profile->deceleration * -dir;

		if(time > timeToDecelerate + timeToExchange)
			return profile->endVelocity;
	  else
			return velocityAtExchange - profile->deceleration * (time - timeToExchange) * dir;
	}
}

float MotionProfileGetVelocityWithMaxVelocity(struct MotionProfile *profile, float time)
{
  // Are we accelerating forward or backwards?
  float dir = 1 - 2*(profile->startVelocity > profile->maxVelocity);

	// Current velocity(if max velocity didn't exist)
	float vel1 = profile->startVelocity + profile->acceleration * time * dir;

	// How long does it take to accelerate to max speed
	float timeToMaxVel = (profile->maxVelocity - profile->startVelocity) / (profile->acceleration)*dir;

	// Calculate distance at current velocity
	float distanceToStop = (vel1 * vel1 - profile->endVelocity * profile->endVelocity) / (2.0 * profile->deceleration)*dir;
	float distanceSoFar = profile->startVelocity * time + 0.5 * profile->acceleration*time*time*dir;

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

#endif
