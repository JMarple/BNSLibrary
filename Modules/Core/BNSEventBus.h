// BNSEventBus.h
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 5/31/2015
//
// This header file includes source code that
// implements an global Event Bus interface
// This allows for loosely coupled software
//   and higher reuability.
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

#ifndef __BNS_EVENT_BUS_H
#define __BNS_EVENT_BUS_H

#ifndef __BNS_DATA_STRUCTURES_H
#include "BNSDataStructures.h"
#endif

#define EventBusSubscribe(a, e, s) _EventBus_Subscribe(a, sizeof(e), s)
#define EventBusPublish(a, e) _EventBus_Publish(a, (intptr_t*)e)
// The EventBus is a singleton, so declare it global
DynamicArray EventBus;
int _EventBusIDCounter;

#define _EventBUSIDCounterDefault 2
struct EventBusBox
{
	// Identifies what kind of messages this specific "mail box" is looking for
	int messageID;

	// A circular buffer of messages.
	CircularBuffer messages;

	// TODO WHEN ROBOTC IMPLEMENTS: Function Pointer to callback function
};

void InitEventBus();
int32_t _EventBus_Subscribe(int32_t messageID, int objectSize, int bufferSize);
int32_t _EventBus_Publish(int32_t messageID, intptr_t* object);
intptr_t EventBusGetMessage(int32_t userID);
#endif
