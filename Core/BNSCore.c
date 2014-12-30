#ifndef __BNS_LIB_H
#include "../BNSLib.h"
#endif

#ifndef __BNS_HEAP_H
#include "BNSHeap.h"
#endif

#ifndef __BNS_CORE_H
#include "BNSCore.h"
#endif

// Anything that needs to be initiated should go here
void BNS()
{
    // Start Heap
    initMemory();

    // Start random number generation
    srand(nSysTime);

    // Output version number
    writeDebugStreamLine("*** Loaded BNSLib V0.11 for RobotC 4.27 ***");
}
