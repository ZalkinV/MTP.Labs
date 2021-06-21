#include <math.h>

#include "TimeMeasurer.h"
#include "MatrixMulImpls.h"
#include "Tests.h"


void measureTime()
{
	size_t sizesCount = 12;
	size_t* sizes = getSizesPowerOfTwo(sizesCount);

	size_t impls[] = { 1, 2, 23, 3 };
	size_t implsCount = sizeof(impls) / sizeof(size_t);

	cl_uint deviceIndex = 1;
	printf("Device,Size,Impl,Kernel ms,Full ms\n");

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
			mtype* result = runMulKernel(deviceIndex, firstMatrix, secondMatrix, rowColCount, rowColCount, rowColCount, implNumber, &kernelExecTime, &fullExecTime);
			printf("%i,%i,%i,%f,%f\n", deviceIndex, rowColCount, implNumber, kernelExecTime, fullExecTime);

			delete[] result;
		}


		delete[] firstMatrix;
		delete[] secondMatrix;
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
