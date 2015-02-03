// SimpleKalmanFilter.h
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 2/2/2014
//
// This header file includes code that
// implements a KalmanFilter for use in
// Vex Robotics Competition.  This is a simple
// version of the original Kalman Filter code that
// assumes a basic update and extraction matrix
//
// Source Code:
//   SimpleKalmanFilter.c
//
// Dependencies:
//    KalmanFilter.h
//    BNSMatrix.h
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

#ifndef __BNS_SIMPLE_KALMAN_FILTER_H
#define __BNS_SIMPLE_KALMAN_FILTER_H

// SimpleKalmanInit(...)
// Initializes the Kalman Filter with fairly typical properties.
// Dt is measured in seconds per update
bool SimpleKalmanInit(struct KalmanFilter *filter, float dt);

// SimpleKalmanUpdate(...)
// Updates a given kalman filter with a 1x1 peice of data
// Essentially you input data and this function will output a
//  better approximation of that data
float SimpleKalmanUpdate(struct KalmanFilter* filter, float data);

// SimpleKalmanGetPosition(...)
// Returns the position from the most recent update
float SimpleKalmanGetPosition(struct KalmanFilter* filter);

// SimpleKalmanGetVelocity(...)
// Returns the velocity from the most recent update
float SimpleKalmanGetVelocity(struct KalmanFilter* filter);

#endif
