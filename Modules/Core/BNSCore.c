#ifndef __BNS_LIB_H
#include "../BNSLib.h"
#endif

#ifndef __BNS_HEAP_H
#include "BNSHeap.h"
#endif

#ifndef __BNS_CORE_H
#include "BNSCore.h"
#endif

#ifndef __BNS_EVENT_BUS_H
#include "BNSEventBus.h"
#endif

// Anything that needs to be initiated should go here
void BNS()
{
    // Start Heap
    initMemory();

    // Start random number generation
    srand(nSysTime);

    // Output version number
    writeDebugStreamLine("*** Loaded BNSLib V0.20.0 for RobotC 4.27 ***");
}

// This is a seperate initiation for the EventBus
//  It is seperated so that it doesn't take up extra space in the heap
//  when the user doesn't want it
void BNSEventBus()
{
		InitEventBus();
}

void BNS_ERROR(char* errorTitle, char* error)
{
    writeDebugStream("***\nBNS ");
    writeDebugStreamLine(errorTitle);
    writeDebugStreamLine(error);
    writeDebugStreamLine("***");
}
