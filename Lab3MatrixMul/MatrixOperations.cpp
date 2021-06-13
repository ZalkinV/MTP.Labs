#define _CRT_SECURE_NO_WARNINGS

#include "MatrixOperations.h"
#include <stdexcept>
#include <string>

using namespace std;


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

mtype* multiplyColumnwise(mtype* matrixA, mtype* matrixB, int rowsCount, int colsCount, int elsCount)
{
	mtype* matrixRes = new mtype[rowsCount * colsCount];

	int xCount = rowsCount;
	int yCount = colsCount;

	for (int iCol = 0; iCol < yCount; iCol++) // Y
	{
		for (int iRow = 0; iRow < xCount; iRow++) // X
		{
			mtype elSum = 0;
			for (int iEl = 0; iEl < elsCount; iEl++)
			{
				int indexA = iRow * elsCount + iEl;
				int indexB = iEl * yCount + iCol;
				mtype elMul = matrixA[indexA] * matrixB[indexB];
				elSum += elMul;
			}

			int indexRes = iRow * yCount + iCol;
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

void readMatrices(
	char* fileName,
	mtype** firstMatrix, mtype** secondMatrix,
	size_t* firstRowsCount, size_t* colsRowsCount, size_t* secondColsCount)
{
	FILE* file = fopen(fileName, "r");
	if (file == NULL)
		throw runtime_error("Cannot open file '" + string(fileName) + "'");

	int err = fscanf(file, "%i %i %i", secondColsCount, colsRowsCount, firstRowsCount);

	*firstMatrix = readMatrix(file, *firstRowsCount, *colsRowsCount);
	*secondMatrix = readMatrix(file, *colsRowsCount, *secondColsCount);

	fclose(file);
}

mtype* readMatrix(FILE* file, const size_t rowsCount, const size_t colsCount)
{
	int err = 0;
	size_t elementsCount = rowsCount * colsCount;
	mtype* matrix = new mtype[elementsCount];
	for (size_t i = 0; i < elementsCount; i++)
	{
		err = fscanf(file, "%f", &matrix[i]);
	}

	return matrix;
}

void printMatrix(const mtype* matrix, const size_t rowsCount, const size_t colsCount)
{
	printMatrixToStream(stdout, matrix, rowsCount, colsCount);
}

void fprintMatrix(const char* fileName, const mtype* matrix, const size_t rowsCount, const size_t colsCount)
{
	FILE* file = fopen(fileName, "w");
	
	printMatrixToStream(file, matrix, rowsCount, colsCount);

	fclose(file);
}

void printMatrixToStream(FILE* stream, const mtype* matrix, const size_t rowsCount, const size_t colsCount)
{
	fprintf(stream, "%i %i\n", colsCount, rowsCount);
	for (size_t iRow = 0; iRow < rowsCount; iRow++)
	{
		for (size_t iCol = 0; iCol < colsCount; iCol++)
		{
			size_t indexC = iRow * colsCount + iCol;
			fprintf(stream, "%f ", matrix[indexC]);
		}
		fprintf(stream, "\n");
	}
}
