#pragma systemFile

void CreateNewMatrix(Matrix mat, int m, int n)
{
	// Dimensions
  mat.m = m;
  mat.n = n;
  mat.bufferLocation = bnsMalloc(m*n);
}

void DeleteMatrix(Matrix mat)
{
	mat.m = 0;
	mat.n = 0;
	bnsFree(mat.bufferLocation);
	mat.bufferLocation = -1;
}

void SetMatrixAt(Matrix mat, int m, int n, float value)
{
	if(m <= mat.m && n <= mat.n)
	{
	  setBufferElement(mat.bufferLocation + mat.n * m + n, value);
	}
}

float GetMatrixAt(Matrix mat, int m, int n)
{
	return getBufferElement(mat.bufferLocation + mat.n * m + n);
}

void PrintMatrix(Matrix A)
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

// Multiply Matricies as AB, saving to dst
bool MultiplyMatrix(Matrix dst, Matrix A, Matrix B)
{
  if(A.n != B.m)
  	return false;

  Matrix tmpCol;
	CreateNewMatrix(tmpCol, B.m, 1);

	// Loop through each row and column of our second matrix
	for(int row = 0; row < A.m; row++)
	{
		for(int col = 0; col < B.n; col++)
		{
			float sum = 0;

			for(int k = 0; k < B.m; k++)
			{
				sum += GetMatrixAt(A, row, k) * GetMatrixAt(B, k, col);
			}

			SetMatrixAt(dst, row, col, sum);
		}
	}

	DeleteMatrix(tmpCol);

	return true;
}

void SwapRowsInMatrix(Matrix A, int row1, int row2)
{
	Matrix tmpRow;
	CreateNewMatrix(tmpRow, 1, A.n);

	// Copy first row
	for(int i = 0; i < A.n; i++)
		SetMatrixAt(tmpRow, 0, i, GetMatrixAt(A, row1, i));

	// Copy second row to first row
	for(int i = 0; i < A.n; i++)
		SetMatrixAt(A, row1, i, GetMatrixAt(A, row2, i));

	// Copy tmpRow to secondRow
	for(int i = 0; i < A.n; i++)
		SetMatrixAt(A, row2, i, GetMatrixAt(tmpRow, 0, i));

	// Free up the space we took to write the tmpRow
	DeleteMatrix(tmpRow);
}

// Searches through all the rows in a single column to find the largest number in it
int GetLargestColumnFromMatrix(Matrix A, int col, int row1, int row2)
{
	int largest = GetMatrixAt(A, row1, col);
	int index = row1;

	for(int i = row1+1; i < row2; i++)
	{
		int val = GetMatrixAt(A, row1, i);
		if(val > largest)
		{
			largest = val;
			index = i;
		}
	}

	return index;
}

// Add Matricies together A+B
bool AddMatrix(Matrix dst, Matrix A, Matrix B)
{
	if(A.m != B.m || A.n != B.n)
		return false;

  for(int i = 0; i < A.m; i++)
  {
  	for(int j = 0; j < A.n; j++)
  	{
  		SetMatrixAt(dst, i, j,
  			GetMatrixAt(A, i, j) + GetMatrixAt(B, i, j)
  		);
  	}
  }

  return true;
}

// Subtract Matricies together A-B
bool SubMatrix(Matrix dst, Matrix A, Matrix B)
{
	if(A.m != B.m || A.n != B.n)
		return false;

  for(int i = 0; i < A.m; i++)
  {
  	for(int j = 0; j < A.n; j++)
  	{
  		SetMatrixAt(dst, i, j,
  			GetMatrixAt(A, i, j) - GetMatrixAt(B, i, j)
  		);
  	}
  }

  return true;
}

float FindMatrixDeterminant(Matrix A)
{
	// Ensure this is a square matrix
	if(A.m != A.n)
		return 0;

	if(A.m == 2)
	{
		return GetMatrixAt(A, 0, 0) * GetMatrixAt(A, 1, 1) - GetMatrixAt(A, 0, 1) * GetMatrixAt(A, 1, 0);
  }
	else if(A.m == 3)
	{
		float firstsum = 0;
		for(int offset = 0; offset < A.m; offset++)
		{
			float product = 1;
			for(int i = 0; i < A.m; i++)
			{
				product *= GetMatrixAt(A, i, (i+offset)%A.m);
			}
			//writeDebugStreamLine("Product = %f", product);
			firstsum += product;
		}

		float secondsum = 0;
		for(int offset = 0; offset < A.m; offset++)
		{
			float product = 1;
			for(int i = A.m-1; i >= 0; i--)
			{
				product *= GetMatrixAt(A, A.m-1-i, (i+offset)%A.m);
			}
			//writeDebugStreamLine("Product = %f", product);
			secondsum += product;
		}

		return firstsum - secondsum;
	}

	return 0;
}

float FindMatrixTrace(Matrix A)
{
	// Ensure this is a square matrix
	if(A.m != A.n)
		return 0;

	// Return Trace
	float trace = 1;
	for(int i = 0; i < A.n; i++)
	{
		trace += GetMatrixAt(A, i, i);
	}

	return trace;
}

bool FindMatrixOfMinors(Matrix dst, Matrix A)
{
	// Ensure this is a square matrix
	if(A.m != A.n)
		return false;

	Matrix tmp;
	CreateNewMatrix(tmp, A.m-1, A.m-1);

	// Go through each i, j to find matrix of minors
	for(int i = 0; i < A.m; i++)
	{
		for(int j = 0; j < A.n; j++)
		{
			int x = 0;

			// Go through each m, n to find determinant
			for(int m = 0; m < A.m-1; m++)
			{
				int y = 0;
				for(int n = 0; n < A.n-1; n++)
				{
					SetMatrixAt(tmp, y, x, GetMatrixAt(A, n+(n>=j), m+(m>=i)));
					y++;
				}
				x++;
			}

			float det = FindMatrixDeterminant(tmp);
			SetMatrixAt(dst, j, i, det);
		}
	}

	DeleteMatrix(tmp);

	return true;
}

bool FindCofactorMatrix(Matrix dst, Matrix A)
{
	// Ensure this is a square matrix
	if(A.m != A.n)
		return false;

	FindMatrixOfMinors(dst, A);

	for(int i = 0; i < dst.m; i++)
	{
		for(int j = 0; j < dst.m; j++)
		{
			if((i+j)%2 == 1)
			{
				SetMatrixAt(dst, i, j, GetMatrixAt(dst, i, j) * -1);
			}
		}
	}

	return true;
}

// Only works for square matricies and vectors atm
void FindTransposeMatrix(Matrix dst, Matrix A)
{
	for(int i = 0; i < A.m-1; i++)
	{
		for(int j = i + 1; j < A.n; j++)
		{
			int posA = GetMatrixAt(A, i, j);
			int posB = GetMatrixAt(A, j, i);
			SetMatrixAt(dst, i, j, posB);
			SetMatrixAt(dst, j, i, posA);
		}
	}

	int m = dst.m;
	dst.m = dst.n;
	dst.n = m;
}

bool FindInverseMatrix(Matrix dst, Matrix A)
{
	// Ensure this is a square matrix
	if(A.m != A.n || dst.m != A.m || dst.n != A.n)
		return false;

  bool realResult = true;

  FindCofactorMatrix(dst, A);
  FindTransposeMatrix(dst, dst);

  float det = FindMatrixDeterminant(A);

  if(det == 0)
  	realResult = false;

	for(int i = 0; i < A.m; i++)
	{
		for(int j = 0; j < A.n; j++)
		{
			SetMatrixAt(dst, i, j, GetMatrixAt(dst, i, j) / det);
		}
	}

	return realResult;
}
