// MotionProfile.h
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 2/3/2015
//
// This header file includes code that
// implements Motion Profiles for use in
// Vex Robotics Competition.
//
// Source Code:
//   MotionProfile.c
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

#ifndef __BNS_MOTION_PROFILE_H
#define __BNS_MOTION_PROFILE_H

struct MotionProfile
{
	float acceleration;
	float deceleration;

	float startVelocity;
	float maxVelocity;
	float endVelocity;

	float distance;
};

// MotionProfileInit(...)
// Initate defaults for the motion profile
void MotionProfileInit(struct MotionProfile *profile);

// MotionProfileEasyParams(...)
// Initiate simple params for the motion params
void MotionProfileEasyParams(struct MotionProfile* profile, float accel, float speed, float distance);

// MotionProfileSetDistance(...)
// Sets the desired distance for the motion controller
void MotionProfileSetDistance(struct MotionProfile *profile, float distance);

// MotionProfileSetAccel(...)
// Sets the desired acceleration/deceleration for the motion controller
void MotionProfileSetAccel(struct MotionProfile *profile, float accel, float decel);

// MotionProfileSetVelocity(...)
// Sets the desired velocities for the motion controller
void MotionProfileSetVelocity(struct MotionProfile *profile, float enterVel, float maxVel, float exitVel);

// MotionProfileIsComplete(...)
// Returns true if the motion profile is complete at a given time variable
bool MotionProfileIsComplete(struct MotionProfile *profile, float time);

// MotionProfileCompute(...)
// Given a time input, return the appropriate speed
// Units are entirely arbritrary and should be determined by the user
// This should be used in a loop with a variable that relates to the real world time
float MotionProfileCompute(struct MotionProfile *profile, float time);

// MotionProfilePrint(...)
// Prints to the debug stream what the profile is going to do.
// Set dt to however detailed you want your profile to print as
// For instance, if you are running the profile at 25 times a second
//  set dt to 1/25, or 0.04 to see what it will return
// Note: This function will take a long time to print, only use it for
//  debugging
void MotionProfilePrint(struct MotionProfile* profile, float dt);

#endif
