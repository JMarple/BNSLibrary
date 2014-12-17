#pragma systemFile

// Obtain space for a matrix filled with all zeros
void CreateZerosMatrix(Matrix mat, int m, int n)
{
  // Dimensions
  mat.m = m;
  mat.n = n;
  mat.inUse = true;
  mat.bufferLocation = bnsMalloc(m*n);

  // Initalize to all zeros
  for(int i = 0; i < m; i++)
    for(int j = 0; j < n; j++)
      SetMatrixAt(mat, i, j, 0);
}

// Obtain space for an identity matrix of size n
void CreateIdentityMatrix(Matrix mat, int n)
{
  // Dimensions
  mat.m = n;
  mat.n = n;
  mat.inUse = true;
  mat.bufferLocation = bnsMalloc(n*n);

  // Initialize with 1's on the main diagonal
  for(int i = 0; i < n; i++)
  {
    for(int j = 0; j < n; j++)
    {
      if(i==j)
        SetMatrixAt(mat, i, j, 1);
      else
        SetMatrixAt(mat, i, j, 0);
    }
  }
}

// This function takes in a string input and determines
//   how many rows and columns the user wants to have
//   ';' means a new row, spaces means new column
bool ParseMatrixString(Matrix mat, char* s)
{
  // Index/counters
  int i = 0;
  int rows = 1;
  int cols = 0;

  // This lets us trigger between numbers and non-numbers
  // When this is true, we are looking at a number,
  //   but when it switchs from true to false, we can
  //   increment the col variable to count the amount of nums
  bool numTrig = false;
  char lastRealChar = 0;// A crap hack, ignore final ';' if accidently inputted.

  // Determine number of rows and columns
  while(s[i] != 0)
  {
    // Semicolons determine new rows
    if(s[i] == ';')
    {
      lastRealChar = ';';
      rows++;
      if(numTrig == true) cols++;

      numTrig = false;
    }
    // If a space, we know it's not a number
    else if(s[i] == ' ')
    {
      if(numTrig == true)	cols++;

      numTrig = false;
    }
    // Look for hidden tabs/enters being parsed
    else if(s[i] == 9 || s[i] == 10){numTrig = false;}
    // Must be a number, so set numTrig true so we can watch
    //  for a non-number
    else
    {
      lastRealChar = ' ';
      numTrig = true;
    }

    // Increment string index
    i++;
  }
  if(numTrig == true)
    cols++;

  // Bleh
  if(lastRealChar == ';')
    rows--;

  if(cols%rows != 0)
  {
    writeDebugStreamLine("%d %d", cols, rows);

    writeDebugStreamLine("***\nBNS MATRIX ERROR\nCannot parse matrix string in CreateMatrix(...), inconsistent rows or columns!\n***\n");
    return false;
  }
  cols /= (int)rows;

  // Set Parameters
  mat.m = rows;
  mat.n = cols;
  mat.inUse = true;
  mat.bufferLocation = bnsMalloc(rows*cols);
  return true;
}

// Obtain space for a matrix given a string as an input
bool CreateMatrix(Matrix mat, char* s)
{
  char* cpy = s;

  // Parse the given string and save the results to mat
  if(!ParseMatrixString(mat, s))
    return false;

  int sIndex = 0;

  // Fill empty matrix with the information
  for(int i = 0; i < mat.m; i++)
  {
    for(int j = 0; j < mat.n; j++)
    {
      char tmp[16];
      for(int i = 0; i < 16; i++)
        tmp[i] = 0;

      int tmpIndex = 0;
      bool numTrig = false;

      while(true)
      {
        if(s[sIndex] >= 45 && s[sIndex] <= 60)
        {
          tmp[tmpIndex] = s[sIndex];
          tmpIndex++;
          numTrig = true;
        }
        else if(numTrig == true)
          break;

        sIndex++;
      }
      string str = "";
      stringFromChars(str, tmp);
      SetMatrixAt(mat, i, j, atof(str));
    }
  }

  return true;
}

// Free space given a matrix
void DeleteMatrix(Matrix mat)
{
  mat.m = 0;
  mat.n = 0;
  if(mat.inUse == true)
  {
    bnsFree(mat.bufferLocation);
    mat.bufferLocation = -1;
  }
}

// Get a copy of a matrix by allocating new space and
//   directly copying all values to the new matrix
bool CopyMatrixByValue(Matrix dst, Matrix src)
{
  DeleteMatrix(dst);
  CreateZerosMatrix(dst, src.m, src.n);

  dst.m = src.m;
  dst.n = src.n;

  for(int i = 0; i < dst.m; i++)
  {
    for(int j = 0; j < dst.n; j++)
    {
      SetMatrixAt(dst, i, j, GetMatrixAt(src, i, j));
    }
  }

  return true;
}

// Get a copy of a matrix by getting a reference to
//  the original data
bool CopyMatrix(Matrix dst, Matrix src)
{
  dst.m = src.m;
  dst.n = src.n;
  dst.bufferLocation = src.bufferLocation;

  return true;
}

// Set an element in a matrix, given m-down and n-across
void SetMatrixAt(Matrix mat, int m, int n, float value)
{
  if(m <= mat.m && n <= mat.n)
  {
    bnsSetHeapElement(mat.bufferLocation + mat.n * m + n, value);
  }
}

// Get an element in a matrix given m-down and n-across
float GetMatrixAt(Matrix mat, int m, int n)
{
  return bnsGetHeapElement(mat.bufferLocation + mat.n * m + n);
}

// Print a Matrix to the console
void PrintMatrix(Matrix A)
{
  if(A.inUse == true)
  {
    writeDebugStream("Matrix:\n");
    for(int i = 0; i < A.m; i++)
    {
      for(int j = 0; j < A.n; j++)
      {
        writeDebugStream("%f\t", GetMatrixAt(A, i, j));
      }
      writeDebugStream("\n");
    }
  }
  else
  {
    writeDebugStreamLine("***\nBNS MATRIX ERROR\nCannot Print an unset or NULL matrix!\n***\n");
  }
}
