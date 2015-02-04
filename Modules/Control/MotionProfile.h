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

#endif
