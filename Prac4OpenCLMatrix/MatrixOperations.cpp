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
