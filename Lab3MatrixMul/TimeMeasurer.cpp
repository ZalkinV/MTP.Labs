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
				
				mtype* expected = multiply(firstMatrix, secondMatrix, rowColCount, rowColCount, rowColCount);
				bool isEqual = equals(expected, result, rowColCount, rowColCount);
				if (!isEqual)
				{
					printf("WRONG! ");
				}

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
