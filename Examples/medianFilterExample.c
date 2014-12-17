// medianFilterExample.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 12/16/2014
//
// This example file shows how to use
// the median filter built into the
// BNS Library
//
// Dependencies:
//   BNSLib.h in previous directory
#include "..\BNSLib.h"

task main()
{
  // Initialize the BNS Library
  BNS();

  // Initiate a median filter of size 5
  // What a median filter does is simply takes in so
  //   many data points and computes the median of them!
  //
  // In this instance, we will be taking the median of the
  //   previous 5 data points that are entered.  This is done
  //   by calling "MedianAddData(filter, ##)".  By calling
  //   MedianCompute(filter), it will return the median of your
  //   data, hence the median filter!
  //
  // This filter is especially good for data you believe will have
  //   spontaneous noise.  A pratical example is the sonar may
  //   return "reflections" at random due to waves bouncing off
  //   walls in weird ways, so this noise can be cut out by
  //   taking the median of the data.
  MedianFilter filter;
  MedianInitialize(filter, 5);

  // Add Random Data, with an outcast 100
  MedianAddData(filter, 2);
  MedianAddData(filter, 5);
  MedianAddData(filter, 100);
  MedianAddData(filter, 2);
  MedianAddData(filter, 4.5);

  // Compute Median Filter, where we have
  // 2  2  4.5  5  100
  // so our median is 4.5
  float result = MedianCompute(filter);
  writeDebugStreamLine("%f", result);

  // Add Random Data, with an outcast of 1000
  MedianAddData(filter, 1000);
  MedianAddData(filter, 8);
  MedianAddData(filter, 4.789);

  // Compute Median Filter, where we have
  // 2  4.5  4.789  8  1000
  // so our median is 4.789
  result = MedianCompute(filter);
  writeDebugStreamLine("%f", result);
}
