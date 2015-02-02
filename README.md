<h2>BNSLib V0.13.3</h2>
This library is designed to be an advanced library for Highschool and University Vex Robotics Teams.  It can be compiled on RobotC 4.27 (older versions may not compile correctly).  The library features several concepts:
 - Dynamic Memory Allocation
 - Complex Matrix Operations (+ - * /, deterimant, trace, and others)
 - Gaussian Distribution Functions
 - Kalman Filter and Median Filter (more to come)
 - PID Controllers
  
This library was designed by Justin Marple on Team BNS
<br/>Contact: jmarple@umass.edu
<br/><h4>Cheat Sheet</h4>
Matrix Basics:
 - CreateMatrix           (Matrix *dst, char *input)
 - CreateZerosMatrix      (Matrix *dst, int height, int width)
 - CreateIdentityMatrix   (Matrix *dst, int height)
 - SetMatrixAt            (Matrix *dst, int height, int width, float value)
 - GetMatrixAt            (Matrix *dst, int height, int width)
 - DeleteMatrix           (Matrix *dst)
 - CopyMatrix             (Matrix *dst, Matrix src)
 - CopyMatrixByValue      (Matrix *dst, Matrix src)
 
Matrix Arithmetic:
 - MatrixAdd              (Matrix *dst, Matrix A, Matrix B)
 - MatrixSub              (Matrix *dst, Matrix A, Matrix B)
 - MatrixMult             (Matrix *dst, Matrix A, Matrix B)
 - MatrixInv              (Matirx *dst, Matrix A)
 
Matrix Operations:
 - MatrixTranspose        (Matrix *dst, Matrix A)
 - MatrixDeterminant      (Matrix *src)
 - MatrixTrace            (Matrix *src)
 
Dynamic Array Basics:
 - DynamicArrayAdd        (DynamicArray *array, float value)
 - DynamicArrayGet        (DynamicArray *array)
 - DynamicArraySize       (DynamicArray *array)
 - DynamicArrayRemoveAt   (DynamicArray *array, float index)
 - DynamicArrayClear      (DynamicArray *array)
 - DynamicArrayDelete     (DynamicArray *array)
 
Stack Basics:
 - StackPush              (Stack *object, float value)
 - StackPop               (Stack *object)
 - StackPeek              (Stack *object)
 - StackIsEmpty           (Stack *object)
 
PID Basics:
 - PIDInit                (PID *controller, float kP, float kI, float kD)
 - PIDCompute             (PID *controller, float error)

Gaussian Distribution Basics:
 - GaussianInit           (Gaussian *gau, Matrix mean, Matrix covariance)
 - GaussianPDF            (Gaussian *gau, Matrix data)
 
Kalman Filter Basics:
 - KalmanInit             (KalmanFilter *kal, int variables, int measurements, [...])
 - KalmanPredict          (KalmanFilter *kal)
 - KalmanUpdate           (KalmanFilter *kal, Matrix measurement)
 
Median Filter Basics:
 - MedianInit             (MedianFilter *filter, float size)
 - MedianAddData          (MedianFilter *filter, float data)
 - MedianCompute          (MedianFilter *filter)
 
<br/><h4>Update Log</h4>
 - V0.10.0: First release, basic matrix operation and filters implemented
 - V0.11.0: Updated for portability with most C compilers (note: still some issues with portability though)
 - V0.12.0: PID controller added.  Changed MultiplyScalarToMatrix to MatrixMultiplyScalar
 - V0.13.0: DynamicArray & Stack Data Structures Added. Added PID Integral functions.  
 - V0.13.3: BNSMatrix changed to use DynamicArray.  Significant bug found where uninitialized structs could seen as initialized. 