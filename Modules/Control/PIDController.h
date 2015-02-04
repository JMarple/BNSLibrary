// PIDController.h
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 01/07/2015
//
// This header file includes code that
// implements Gaussian distributions for use in
// Vex Robotics Competition.  These are used for
// many probabilist algorithms and can tell you
// certainty of a measurement
//
// Source Code:
//   PIDController.c
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

#ifndef __BNS_PID_CONTROLLER_H
#define __BNS_PID_CONTROLLER_H

// Structure that holds all PID values
struct PID
{
    // This is the general error value
  	// Increase this when you want the response to try to overshoot more
    float kP;

    // This is the integral value (usually recommended to be left zero)
    // This should be added when the PID controller can't reach a certain goal point
    //  after a long period of time.  For instance, say you have a lift that tries to reach
    //  18 inches, but due to the P and D constants, it slows down at 17 inches.  Now, it may
    //  stall at 17 inches and never reach 18 inches because it's too "close" to the target.
    //  By adding the integral, you allow the program to detect that.
    float kI;

    // This is the derivative value
    // Increase this when you want more dampening in your PID response. For instance, you want
    //  your dive to stop at a certain location without overshooting and having to go in reverse
    //  to correct itself
    float kD;

    // Limit for how high/low the integral variable can go before beging cut off.
    float kILimit;

    // Various variables to keep track of for the PID Controller
    float error;
    float previousError;
    float integral;
    float derivative;
};

// PIDInit(...)
// Initalize the PID controller to default values given P, I, D constants
void PIDInit(struct PID *controller, float kP, float kI, float kD);

// PIDCompute(...)
// Computes the response of the PID given an error value
float PIDCompute(struct PID *controller, float error);

// PIDSetIntegralLimit(...)
// Sets the limit for how large for the integral value can become
void PIDSetIntegralLimit(struct PID *controller, float kILimit);

// PIDResetIntegral(...)
// Resets the integral value to zero
void PIDResetIntegral(struct PID *controller);

#endif
