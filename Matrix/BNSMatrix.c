#pragma systemFile

void CreateZerosMatrix(Matrix mat, int m, int n)
{
	// Dimensions
	mat.m = m;
	mat.n = n;
	mat.inUse = true;
	mat.bufferLocation = bnsMalloc(m*n);

	for(int i = 0; i < m; i++)
		for(int j = 0; j < n; j++)
		SetMatrixAt(mat, i, j, 0);
}

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

	// Determine number of rows and columns
	while(s[i] != 0)
	{
		// Semicolons determine new rows
		if(s[i] == ';')
		{
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
			numTrig = true;
		}

		// Increment string index
		i++;
	}
	if(numTrig == true)
		cols++;

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

void CreateMatrix(Matrix mat, char* s)
{
	char* cpy = s;

	// Parse the given string and save the results to mat
	if(!ParseMatrixString(mat, s))
		return;

	int sIndex = 0;

	for(int i = 0; i < mat.m; i++)
	{
		for(int j = 0; j < mat.n; j++)
		{
			char tmp[16];
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
}

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

bool CopyMatrixToMatrix(Matrix dst, Matrix src)
{
	// Check we have enough space to copy
	if(dst.m*dst.n != src.m*src.n)
	{
		writeDebugStreamLine("***\nBNS MATRIX ERROR\nCannot copy a %d, %d to a %d, %d matrix, not the same number of cells\n***\n", src.m, src.n, dst.m, dst.n);
		return false;
	}

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

bool CopyMatrix(Matrix dst, Matrix src)
{
	dst.m = src.m;
	dst.n = src.n;
	dst.bufferLocation = src.bufferLocation;

	return true;
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
	{
		writeDebugStreamLine("***\nBNS MATRIX ERROR\nCannot Multiply a %d, %d to a %d, %d matrix, incorrect sizes\n***\n", A.m, A.n, B.m, B.n);
		return false;
	}

	Matrix tmpCol;
	CreateZerosMatrix(tmpCol, B.m, 1);

	Matrix dstCopy;
	CreateZerosMatrix(dstCopy, A.m, B.n);

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

			SetMatrixAt(dstCopy, row, col, sum);
		}
	}

	// Copy our results matrix to our destination matrix
	DeleteMatrix(dst);
	CreateZerosMatrix(dst, dstCopy.m, dstCopy.n);
	CopyMatrixToMatrix(dst, dstCopy);

	DeleteMatrix(tmpCol);
	DeleteMatrix(dstCopy);

	return true;
}

void SwapRowsInMatrix(Matrix A, int row1, int row2)
{
	Matrix tmpRow;
	CreateZerosMatrix(tmpRow, 1, A.n);

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
	{
		writeDebugStreamLine("***\nBNS MATRIX ERROR\nCannot Add a %d, %d to a %d, %d matrix, not the same size\n***\n", A.m, A.n, B.m, B.n);
		return false;
	}

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
	{
		writeDebugStreamLine("***\nBNS MATRIX ERROR\nCannot Subtract a %d, %d to a %d, %d matrix, not the same size\n***\n", A.m, A.n, B.m, B.n);
		return false;
	}
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
	{
		writeDebugStreamLine("***\nBNS MATRIX ERROR\nCannot find a determinant of a %d by %d matrix, not a square\n***\n", A.m, A.n);
		return 0;
	}
	if(A.m == 1)
	{
		return GetMatrixAt(A, 0, 0);
	}
	else if(A.m == 2)
	{
		return GetMatrixAt(A, 0, 0) * GetMatrixAt(A, 1, 1) - GetMatrixAt(A, 0, 1) * GetMatrixAt(A, 1, 0);
	}
	else
	{
		float sumOfDet = 0;
		Matrix detMat;
		CreateZerosMatrix(detMat, A.m-1, A.n-1);

		int multiplier = 1;
		for(int col = 0; col < A.n; col++)
		{
			int index = 0;
			for(int i = 0; i < A.n; i++)
			{
				// If we aren't skipping this column
				if(col != i)
				{
					for(int row = 1; row < A.m; row++)
					{
						SetMatrixAt(detMat, row-1, index, GetMatrixAt(A, row, i));
					}
					index++;
				}
			}

			sumOfDet += multiplier * GetMatrixAt(A, 0, col) * FindMatrixDeterminant(detMat);
			multiplier *= -1;
		}

		DeleteMatrix(detMat);

		return sumOfDet;
	}

	return 0;
}

float FindMatrixTrace(Matrix A)
{
	// Ensure this is a square matrix
	if(A.m != A.n)
	{
		writeDebugStreamLine("***\nBNS MATRIX ERROR\nCannot find the trace of a %d by %d matrix, not a square matrix!\n***\n", A.m, A.n);
		return 0;
	}

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
	CreateZerosMatrix(tmp, A.m-1, A.m-1);

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
	Matrix tmpDst;
	CreateZerosMatrix(tmpDst, A.n, A.m);

	for(int i = 0; i < A.m; i++)
	{
		for(int j = 0; j < A.n; j++)
		{
			SetMatrixAt(tmpDst, j, i, GetMatrixAt(A, i, j));
		}
	}

	DeleteMatrix(dst);
	CreateZerosMatrix(dst, tmpDst.m, tmpDst.n);
	CopyMatrixToMatrix(dst, tmpDst);

	DeleteMatrix(tmpDst);
}

bool FindInverseMatrix(Matrix dst, Matrix A)
{
	// Ensure this is a square matrix
	if(A.m != A.n)
		return false;

	Matrix dstTmp;
	CreateZerosMatrix(dstTmp, A.m, A.n);

	bool realResult = true;

	FindCofactorMatrix(dstTmp, A);

	FindTransposeMatrix(dstTmp, dstTmp);

	float det = FindMatrixDeterminant(A);

	if(det == 0)
		realResult = false;

	for(int i = 0; i < A.m; i++)
	{
		for(int j = 0; j < A.n; j++)
		{
			SetMatrixAt(dstTmp, i, j, GetMatrixAt(dstTmp, i, j) / det);
		}
	}

	DeleteMatrix(dst);
	CreateZerosMatrix(dst, A.m, A.n);
	CopyMatrixToMatrix(dst, dstTmp);
	DeleteMatrix(dstTmp);

	return realResult;
}
