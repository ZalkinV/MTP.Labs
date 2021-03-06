#include <math.h>
#include <stdlib.h>

#include "TimeMeasurer.h"
#include "MatrixMulImpls.h"
#include "Tests.h"


void measureTime()
{
	srand(0);


	size_t sizesCount = 12;
	size_t* sizes = getSizesPowerOfTwo(sizesCount);

	size_t impls[] = { 1, 2, 23, 3 };
	size_t implsCount = sizeof(impls) / sizeof(size_t);

	printf("Device,Size,Impl,Kernel ms,Full ms\n");
	for (cl_uint iDevice = 0; iDevice < 2; iDevice++)
	{
		for (int iSize = 0; iSize < sizesCount; iSize++)
		{
			size_t rowColCount = sizes[iSize];
			mtype* firstMatrix = createRandMatrix(rowColCount, rowColCount);
			mtype* secondMatrix = createRandMatrix(rowColCount, rowColCount);

			for (int iImpl = 0; iImpl < implsCount; iImpl++)
			{
				int implNumber = impls[iImpl];

				float kernelExecTime = 0;
				float fullExecTime = 0;
				mtype* result = runMulKernel(iDevice, firstMatrix, secondMatrix, rowColCount, rowColCount, rowColCount, implNumber, &kernelExecTime, &fullExecTime);
				checkCorrectness(result, firstMatrix, secondMatrix, rowColCount, rowColCount, rowColCount);

				printf("%i,%i,%i,%f,%f\n", iDevice, rowColCount, implNumber, kernelExecTime, fullExecTime);


				delete[] result;
			}

			delete[] firstMatrix;
			delete[] secondMatrix;
		}
	}

	delete[] sizes;
}

size_t* getSizesPowerOfTwo(size_t len)
{
	size_t* sizes = new size_t[len];
	for (size_t i = 0; i < len; i++)
	{
		sizes[i] = (size_t)pow(2, i);
	}

	return sizes;
}

void checkCorrectness(mtype* actual, mtype* firstMatrix, mtype* secondMatrix, size_t rowsCount, size_t colsRowsCount, size_t colsCount)
{
	mtype* expected = multiply(firstMatrix, secondMatrix, rowsCount, colsRowsCount, colsCount);
	bool isEqual = equals(expected, actual, rowsCount, colsCount);
	if (!isEqual)
	{
		printf("WRONG! ");
	}
}
