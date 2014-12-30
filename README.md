<h2>BNSLib V0.11</h2>
This library is designed to be an advanced library for Highschool and University Vex Robotics Teams.  It can be compiled on RobotC 4.27 (older versions may not compile correctly).  The library features several concepts:
 - Dynamic Memory Allocation
 - Complex Matrix Operations (+ - * /, deterimant, trace, and others)
 - Gaussian Distribution Functions
 - Kalman Filter and Median Filter (more to come)
  
This library was designed by Justin Marple on Team BNS
<br/>Contact: jmarple@umass.edu
<br/><h4>Cheat Sheet</h4>
Matrix Basics:
 - CreateMatrix(...)
 - CreateZerosMatrix(...)
 - CreateIdentityMatrix(...)
 - SetMatrixAt(...)
 - GetMatrixAt(...)
 - DeleteMatrix(...)
 - CopyMatrix(...)
 - CopyMatrixByValue(...)
 
Matrix Arithmetic:
 - MatrixAdd(...)
 - MatrixSub(...)
 - MatrixMult(...)
 - MatrixInv(...)
 
Matrix Operations:
 - MatrixTranspose(...)
 - MatrixDeterminant(...)
 - MatrixTrace(...)

Gaussian Distribution Basics:
 - GaussianInit(...)
 - GaussianPDF(...)
 
Kalman Filter Basics:
 - KalmanInit(...)
 - KalmanPredict(...)
 - KalmanUpdate(...)
 
Median Filter Basics:
 - MedianInit(...)
 - MedianAddData(...)
 - MedianCompute(...)
 
<br/><h4>Update Log</h4>
 - V0.10: First release, basic matrix operation and filters implemented
 - V0.11: Updated for portability with most C compilers