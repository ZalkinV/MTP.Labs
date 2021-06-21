#include <math.h>
#include <stdio.h>

#include "TimeMeasurer.h"
#include "PrefixSumCalc.h"
#include "Tests.h"
#include <stdlib.h>
#include "Timer.h"


void measureTime()
{
	srand(0);


	size_t lengthsCount = 12;
	size_t* lengths = getSizesPowerOfTwo(lengthsCount);

	size_t localGroupSizes[] = { 2, 8, 64 };
	size_t localGroupSizesCount = sizeof(localGroupSizes) / sizeof(size_t);

	size_t chunksCounts[] = { 2, 8, 64 };
	size_t chunksCountsCount = sizeof(chunksCounts) / sizeof(size_t);

	printf("Device,Length,LocalGroupSize,ChunksCount,Kernel ms,Full ms,CPU ms\n");
	for (cl_uint iDevice = 0; iDevice < 2; iDevice++)
	{
		for (size_t iSize = 0; iSize < lengthsCount; iSize++)
		{
			size_t length = lengths[iSize];
			float* arr = generateRandomArray(length);

			for (size_t iLocalGroupSize = 0; iLocalGroupSize < localGroupSizesCount; iLocalGroupSize++)
			{
				size_t localGroupSize = localGroupSizes[iLocalGroupSize];

				for (size_t iChunksCount = 0; iChunksCount < chunksCountsCount; iChunksCount++)
				{
					size_t chunksCount = chunksCounts[iChunksCount];

					float kernelExecTime = 0;
					float fullExecTime = 0;
					float* result = calcPrefixSum(iDevice, arr, length, localGroupSize, chunksCount, &kernelExecTime, &fullExecTime);
					
					Timer timer;
					timer.start();
					float* sequentialResult = calcPrefixSumSequential(arr, length);
					timer.stop();
					float cpuExecTime = timer.getMs();
					
					checkCorrectness(result, sequentialResult, length);

					printf("%i,%i,%i,%i,%f,%f,%f\n", iDevice, length, localGroupSize, chunksCount, kernelExecTime, fullExecTime, cpuExecTime);

					delete[] result;
					delete[] sequentialResult;
				}
			}

			delete[] arr;
		}
	}

	delete[] lengths;
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

void checkCorrectness(float* actual, float* expected, size_t length)
{
	bool isEqual = equals(expected, actual, length);
	if (!isEqual)
	{
		printf("WRONG! ");
	}
}
