#include <math.h>
#include <stdio.h>

#include "Tests.h"
#include "ArrayOperations.h"
#include "PrefixSumCalc.h"
#include <stdlib.h>


void runTest(float* arr, size_t arrLength, size_t localGroupSize, size_t chunksCount)
{
	float kernelExecTime = 0;
	float fullElapsedTime = 0;

	float* expected = calcPrefixSumSequential(arr, arrLength);
	float* actual = calcPrefixSum(1, arr, arrLength, localGroupSize, chunksCount, &kernelExecTime, &fullElapsedTime);

	bool isEquals = equals(expected, actual, arrLength);
	if (!isEquals)
	{
		printWrongArraysInfo(expected, actual, arrLength);
	}
	else
	{
		printf("len=%i lgs=%i cc=%i passed in kt=%fms ft=%fms!\n", arrLength, localGroupSize, chunksCount, kernelExecTime, fullElapsedTime);
	}

	delete[] expected;
	delete[] actual;
}

void startTests()
{
	srand(0);

	//testLabExample();
	testSwanExample();
	testRandomOnDifferentSizes();
}

void testLabExample()
{
	int len = 5;
	float arr[] = { 1, 2, 3, 4, 5 };
	runTest(arr, len, 4, 2);
}

void testSwanExample()
{
	int len = 16;
	float arr[] = { 1, 4, 2, 0, 2, 3, 1, 5, 2, 3, 6, 1, 5, 3, 2, 3 };
	runTest(arr, len, 2, 2);
}

void testRandomOnDifferentSizes()
{
	size_t sizes[] = { 1, 2, 4, 7, 8, 9, 15, 16, 17, 21, 32, 33, 41, 57, 101, 128, 237 };

	size_t sizesLength = sizeof(sizes) / sizeof(size_t);
	for (int i = 0; i < sizesLength; i++)
	{
		float arrLength = sizes[i];
		float* arr = generateRandomArray(arrLength);
		
		runTest(arr, arrLength, 32, 8);

		delete[] arr;
	}
}

float* generateRandomArray(size_t length)
{
	float* arr = new float[length];
	for (int i = 0; i < length; i++)
	{
		arr[i] = rand() % 10;
	}

	return arr;
}

float* generateSequentialArray(size_t length)
{
	float* arr = new float[length];
	for (int i = 0; i < length; i++)
	{
		arr[i] = i;
	}

	return arr;
}

bool equals(float* firstArr, float* secondArr, size_t length)
{
	for (size_t i = 0; i < length; i++)
	{
		if (fabs(firstArr[i] - secondArr[i]) > CL_FLT_EPSILON)
		{
			printf("First diff at: %i\n", i);
			return false;
		}
	}

	return true;
}

void printWrongArraysInfo(float* expected, float* actual, size_t length)
{
	printf("\n-----WRONG RESULT-----\n");
	printf("length=%i\n", length);
	printf("Actual:\n");
	printArray(actual, length);
	printf("Expected:\n");
	printArray(expected, length);
	printf("----------------------\n");
}
