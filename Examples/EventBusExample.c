// EventBus.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 05/31/2015
//
// This example file shows how to use
// the EventBus interface in BNSLib
// Note: The Task EventPrinter() recieves
//   messages, but doesn't know (and doesn't
//   care) where those messages come from
//   This is why using an event bus creates
//   code that is loosely coupled.
//
// Dependencies:
//   BNSLib.h in previous directory
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

#include "..\BNSLib.h"

task EventPrinter()
{
	// Subscribe to the event bus
	// Message ID = 0x01/0x02
	// Messgae type = char/string
	// Max number of messages to save = 50
	int userID1 = EventBusSubscribe(0x01, char, 50);
	int userID2 = EventBusSubscribe(0x02, string, 50);
	int userID3 = EventBusSubscribe(0x02, string, 50);

	while(1==1)
	{
		// Check for messages on userID1
		char* pChar = EventBusGetMessage(userID1);
		if(pChar != 0)
			writeDebugStreamLine("Message from User 1: %d", *pChar);

	  // Check for messages on userID2
		string* pString1 = (string*)EventBusGetMessage(userID2);
		if(pString1 != 0)
			writeDebugStreamLine("Message from User 2: %s", *pString1);

		// Check for messages on userID3
		string* pString2 = (string*)EventBusGetMessage(userID3);
		if(pString2 != 0)
			writeDebugStreamLine("Message from User 3: %s", *pString2);

		delay(100);
	}
}

task main()
{
	// Initialize BNSLib and turn on the event bus
	BNS();
	BNSEventBus();

	// Start the task that prints messages out to the debug stream
	startTask(EventPrinter);

	// Allow the task to subscribe to the event bus
	//  and then send some messages
	delay(100);
	char alpha = 42;
	string helloWorld = "Hello World";
	string foofoofoo = "foo foo foo!";
  EventBusPublish(0x01, &alpha);
  EventBusPublish(0x02, &helloWorld);
  EventBusPublish(0x02, &foofoofoo);
}
