#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string>
#include <stdexcept>

#include "ArrayOperations.h"

using namespace std;


float* readArray(const char* fileName, size_t* length)
{
	FILE* file = fopen(fileName, "r");
	if (file == NULL)
		throw runtime_error("Cannot open file '" + string(fileName) + "'");

	int n = 0;
	int err = fscanf(file, "%i", &n);
	if (n >= 0)
		*length = n;
	else
		throw runtime_error("Array length cannot be negative");

	float* arr = new float[n];
	for (size_t i = 0; i < n; i++)
	{
		err = fscanf(file, "%f", &arr[i]);
	}

	return arr;
}

void printArray(const float* arr, const size_t length)
{
	printArrayToStream(stdout, arr, length);
}

void fprintArray(const char* fileName, const float* arr, const size_t length)
{
	FILE* file = fopen(fileName, "w");

	printArrayToStream(file, arr, length);

	fclose(file);
}

void printArrayToStream(FILE* stream, const float* arr, const size_t length)
{
	for (size_t i = 0; i < length; i++)
	{
		fprintf(stream, "%f ", arr[i]);
	}
	fprintf(stream, "\n");
}

void printStageArray(const char* stage, float* arr, size_t length)
{
#ifdef LOG_INFO
	printf("Stage %s result:\n", stage);
	printArray(arr, length);
	printf("\n");
#endif
}
