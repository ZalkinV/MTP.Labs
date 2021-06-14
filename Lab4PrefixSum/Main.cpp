#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <stdexcept>

#include "ArrayOperations.h"
#include "PrefixSumCalc.h"

using namespace std;


void labTask(int argc, char* argv[])
{
	if (argc < 4)
		throw exception("Wrong count of arguments");

	int deviceIndex = stoi(argv[1]);
	char* inputFileName = argv[2];
	char* outputFileName = argv[3];

	size_t arrLength = 0;
	float* inputArr = readArray(inputFileName, &arrLength);


	float kernelExecTime = 0;
	float fullElapsedTime = 0;
	size_t localGroupSize = 8;
	size_t chunksCount = 2;
	float* resultArr = calcPrefixSum(deviceIndex, inputArr, arrLength, localGroupSize, chunksCount, &kernelExecTime, &fullElapsedTime);
	printf("\nTime: %f\t%f\n", kernelExecTime, fullElapsedTime);
	fprintArray(outputFileName, resultArr, arrLength);


	delete[] inputArr;
}

int main(int argc, char* argv[])
{
	try
	{
		labTask(argc, argv);
	}
	catch (const exception& e)
	{
		fprintf(stderr, "%s\n", e.what());
		return 1;
	}

	return 0;
}
