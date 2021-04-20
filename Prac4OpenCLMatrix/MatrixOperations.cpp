#include "MatrixOperations.h"

int** multiply(int** matrixA, int** matrixB, int rowsCount, int colsCount, int elsCount)
{
	int** matrixRes = new int* [rowsCount];
	for (int iRow = 0; iRow < rowsCount; iRow++)
		matrixRes[iRow] = new int[colsCount];

	for (int iRow = 0; iRow < rowsCount; iRow++)
	{
		for (int iCol = 0; iCol < colsCount; iCol++)
		{
			int elSum = 0;
			for (int iEl = 0; iEl < elsCount; iEl++)
			{
				int elMul = matrixA[iRow][iEl] * matrixB[iEl][iCol];
				elSum += elMul;
			}

			matrixRes[iRow][iCol] = elSum;
		}
	}

	return matrixRes;
}

int* multiply(int* matrixA, int* matrixB, int rowsCount, int colsCount, int elsCount)
{
	int* matrixRes = new int[rowsCount * colsCount];

	for (int iRow = 0; iRow < rowsCount; iRow++)
	{
		for (int iCol = 0; iCol < colsCount; iCol++)
		{
			int elSum = 0;
			for (int iEl = 0; iEl < elsCount; iEl++)
			{
				int indexA = iRow * rowsCount + iEl;
				int indexB = iEl * colsCount + iCol;
				int elMul = matrixA[indexA] * matrixB[indexB];
				elSum += elMul;
			}

			int indexRes = iRow * rowsCount + iCol;
			matrixRes[indexRes] = elSum;
		}
	}

	return matrixRes;
}

int* convertTo1D(int** matrix, int rowsCount, int colsCount)
{
	int* matrix1D = new int[rowsCount * colsCount];
	for (int iRow = 0; iRow < rowsCount; iRow++)
	{
		for (int iCol = 0; iCol < colsCount; iCol++)
		{
			matrix1D[iRow * rowsCount + iCol] = matrix[iRow][iCol];
		}
	}
	
	return matrix1D;
}
