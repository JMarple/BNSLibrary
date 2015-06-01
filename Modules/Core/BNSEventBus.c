// BNSEventBus.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 5/31/2015
//
// This source file includes source code that
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
#include "BNSEventBus.h"
#endif

void InitEventBus()
{
	// Initialize the EventBus Array as "EventBusBox" objects (found in BNSEventBus.h)
	DynamicArrayInit(&EventBus, EventBusBox);

	// The first "ID" defaults to two
	_EventBusIDCounter = _EventBUSIDCounterDefault;
}

// This function will create a new CircularBuffer object that keeps tracks
//  of messages.  It returns an ID number for each part of code to
//  identify themselves and get messages.  ObjectSize is how many bytes
//  is needed for each message
//
// Unfortuently until RobotC allows for function pointers, there can be
//  no interrupt routines to get messages, only polling.
int32_t _EventBus_Subscribe(int32_t messageID, int objectSize, int bufferSize)
{
	// This function tells our EventBus that there is a new user and to create a new
  //  "mail box" for that user.  The user is then given their userID.  The user supplies
  //  a messageID and any message with that ID will be saved a unique copy of that message
  //  to all the mail boxes that match that ID.

	// Get the specific EventBusBox for this user
  DynamicArrayAddEmpty(&EventBus);
	EventBusBox* box = (EventBusBox*)DynamicArrayGet(&EventBus, _EventBusIDCounter - _EventBUSIDCounterDefault);

	_CircularBuffer_Init(&box->messages, objectSize, bufferSize);
	box->messageID = messageID;

	int userID = _EventBusIDCounter;
	_EventBusIDCounter++;

	return userID;
}


// This function searches through all the open "mail boxes" and finds ones with message ID's
//  that match the one given to the function.  If it does, it adds
int32_t _EventBus_Publish(int32_t messageID, intptr_t* object)
{
	int counter = 0;

	for(int i = 0; i < DynamicArraySize(&EventBus); i++)
	{
		EventBusBox* box = (EventBusBox*)DynamicArrayGet(&EventBus, i);

		if(box->messageID == messageID)
		{
			// Add message to "messages" if not full
			if(CircularBufferIsFull(box->messages))
			{
				BNS_ERROR("EVENT BUS", "EVENT BUS IS FULL!  EXPAND EVENT BUS MEMORY IN EventBusSubscribe(,,###)!");
		  }
			else
		  {
				CircularBufferAdd(box->messages, object);
				counter++;
			}
		}
	}

	// Return how many "mail boxes" the given message went to
	return counter;
}

// Returns a pointer if there was a message, returns 0 if not
intptr_t EventBusGetMessage(int32_t userID)
{
	EventBusBox* box = (EventBusBox*)DynamicArrayGet(&EventBus, userID-_EventBUSIDCounterDefault);

	if(CircularBufferIsEmpty(&box->messages))
		return 0;
  else
	  return CircularBufferGet(&box->messages);
}
