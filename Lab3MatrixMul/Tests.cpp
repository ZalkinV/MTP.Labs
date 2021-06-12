#include <stdlib.h>

#include "Tests.h"
#include "MatrixMulImpls.h"
#include "MatrixOperations.h"


void startTests()
{
	testThirdImplOn1x1xNSize(17);
	//testOnDifferentSizes();
}

void runTest(int implNumber, mtype* firstMatrix, mtype* secondMatrix, size_t rowsCount, size_t colsRowsCount, size_t colsCount)
{
	float kernelTime = 0; float fullTime = 0;
	mtype* expected = multiply(firstMatrix, secondMatrix, rowsCount, colsCount, colsRowsCount);
	mtype* actual = runMulKernel(1, firstMatrix, secondMatrix, rowsCount, colsRowsCount, colsCount, implNumber, &kernelTime, &fullTime);

	bool isEqual = equals(expected, actual, rowsCount, colsCount);
	if (!isEqual)
	{
		printWrongMatricesInfo(expected, actual, firstMatrix, secondMatrix, rowsCount, colsRowsCount, colsCount);
	}
	else
	{
		printf("%ix%ix%i passed!\n", rowsCount, colsRowsCount, colsCount);
	}
}

void testOnDifferentSizes()
{
	srand(0);

	size_t sizes[][3] =
	{
		{1, 1, 1},
		{1, 4, 1},
		{1, 2, 3},
		{2, 2, 2},
		{2, 4, 7},
		{7, 3, 5},
		{17, 1, 17}, // FAIL on indexes >= 17 for 3 impl
		{33, 1, 1}, // FAIL on indexes >= 17 for 3 impl
		{1, 1, 17}, // FAIL on indexes >= 17 for 3 impl
	};

	size_t sizesCount = sizeof(sizes) / sizeof(*sizes);


	for (size_t i = 0; i < sizesCount; i++)
	{
		size_t rowsCount = sizes[i][0];
		size_t colsRowsCount = sizes[i][1];
		size_t colsCount = sizes[i][2];

		mtype* firstMatrix = createRandMatrix(rowsCount, colsRowsCount);
		mtype* secondMatrix = createRandMatrix(colsRowsCount, colsCount);

		runTest(3, firstMatrix, secondMatrix, rowsCount, colsRowsCount, colsCount);

		delete[] firstMatrix;
		delete[] secondMatrix;
	}
}

void testThirdImplOn1x1xNSize(size_t colsCount)
{
	size_t rowsCount = 1;
	size_t colsRowsCount = 1;

	mtype* firstMatrix = new mtype[rowsCount * colsRowsCount] { 2 };
	mtype* secondMatrix = createSequentialMatrix(colsRowsCount, colsCount);

	runTest(3, firstMatrix, secondMatrix, rowsCount, colsRowsCount, colsCount);

	delete[] firstMatrix;
	delete[] secondMatrix;
}

mtype* createRandMatrix(size_t rowsCount, size_t colsCount)
{
	size_t length = rowsCount * colsCount;
	mtype* matrix = new mtype[length];

	for (size_t i = 0; i < length; i++)
	{
		matrix[i] = rand() % 10;
	}

	return matrix;
}

mtype* createSequentialMatrix(size_t rowsCount, size_t colsCount)
{
	size_t length = rowsCount * colsCount;
	mtype* matrix = new mtype[length];

	for (size_t i = 0; i < length; i++)
	{
		matrix[i] = i;
	}

	return matrix;
}

bool equals(mtype* firstMatrix, mtype* secondMatrix, size_t rowsCount, size_t colsCount)
{
	size_t length = rowsCount * colsCount;

	for (size_t i = 0; i < length; i++)
	{
		if (firstMatrix[i] != secondMatrix[i])
		{
			size_t iRow = i / colsCount;
			size_t iCol = i % colsCount;
			printf("First error at: %i %i\n", iRow, iCol);
			return false;
		}
	}

	return true;
}

void printWrongMatricesInfo(mtype* expected, mtype* actual, mtype* first, mtype* second, size_t rowsCount, size_t colsRowsCount, size_t colsCount)
{
	printf("\n-----WRONG RESULT-----\n");
	printf("Actual:\n");
	printMatrix(actual, rowsCount, colsCount);
	printf("Expected:\n");
	printMatrix(expected, rowsCount, colsCount);
	printf("\n");
	printf("First:\n");
	printMatrix(first, rowsCount, colsRowsCount);
	printf("Second:\n");
	printMatrix(second, colsRowsCount, colsCount);
	printf("----------------------\n");
}
