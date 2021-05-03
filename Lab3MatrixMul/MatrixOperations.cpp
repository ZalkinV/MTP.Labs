#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include "MatrixOperations.h"


mtype** multiply(mtype** matrixA, mtype** matrixB, int rowsCount, int colsCount, int elsCount)
{
	mtype** matrixRes = new mtype* [rowsCount];
	for (int iRow = 0; iRow < rowsCount; iRow++)
		matrixRes[iRow] = new mtype[colsCount];

	for (int iRow = 0; iRow < rowsCount; iRow++)
	{
		for (int iCol = 0; iCol < colsCount; iCol++)
		{
			mtype elSum = 0;
			for (int iEl = 0; iEl < elsCount; iEl++)
			{
				mtype elMul = matrixA[iRow][iEl] * matrixB[iEl][iCol];
				elSum += elMul;
			}

			matrixRes[iRow][iCol] = elSum;
		}
	}

	return matrixRes;
}

mtype* multiply(mtype* matrixA, mtype* matrixB, int rowsCount, int colsCount, int elsCount)
{
	mtype* matrixRes = new mtype[rowsCount * colsCount];

	for (int iRow = 0; iRow < rowsCount; iRow++)
	{
		for (int iCol = 0; iCol < colsCount; iCol++)
		{
			mtype elSum = 0;
			for (int iEl = 0; iEl < elsCount; iEl++)
			{
				int indexA = iRow * elsCount + iEl;
				int indexB = iEl * colsCount + iCol;
				mtype elMul = matrixA[indexA] * matrixB[indexB];
				elSum += elMul;
			}

			int indexRes = iRow * colsCount + iCol;
			matrixRes[indexRes] = elSum;
		}
	}

	return matrixRes;
}

mtype* convertTo1D(mtype** matrix, int rowsCount, int colsCount)
{
	mtype* matrix1D = new mtype[rowsCount * colsCount];
	for (int iRow = 0; iRow < rowsCount; iRow++)
	{
		for (int iCol = 0; iCol < colsCount; iCol++)
		{
			int index1D = iRow * colsCount + iCol;
			matrix1D[index1D] = matrix[iRow][iCol];
		}
	}
	
	return matrix1D;
}

void printMatrix(mtype* matrix, int rowsCount, int colsCount)
{
	printf("%i %i\n", colsCount, rowsCount);
	for (int iRow = 0; iRow < rowsCount; iRow++)
	{
		for (int iCol = 0; iCol < colsCount; iCol++)
		{
			int indexC = iRow * colsCount + iCol;
			printf("%f ", matrix[indexC]);
		}
		printf("\n");
	}
}

void fprintMatrix(char* fileName, mtype* matrix, int rowsCount, int colsCount)
{
	FILE* file = fopen(fileName, "w");
	
	fprintf(file, "%i %i\n", colsCount, rowsCount);
	for (int iRow = 0; iRow < rowsCount; iRow++)
	{
		for (int iCol = 0; iCol < colsCount; iCol++)
		{
			int indexC = iRow * colsCount + iCol;
			fprintf(file, "%f ", matrix[indexC]);
		}
		fprintf(file, "\n");
	}

	fclose(file);
}
