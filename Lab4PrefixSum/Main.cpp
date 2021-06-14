#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <stdexcept>

#include "ArrayOperations.h"
#include "PrefixSumCalc.h"

using namespace std;

/*
* Useful links:
* Optimization, Specification and Verification of the Prefix Sum Program in an OpenCL Environment https://fmt.ewi.utwente.nl/media/147.pdf
* Parallel Prefix Sum on the GPU (Scan) http://users.umiacs.umd.edu/~ramani/cmsc828e_gpusci/ScanTalk.pdf
* Chapter 39. Parallel Prefix Sum (Scan) with CUDA https://developer.nvidia.com/gpugems/gpugems3/part-vi-gpu-computing/chapter-39-parallel-prefix-sum-scan-cuda
* Lab: Computing a prefix sum in OpenCL https://www.cs.swan.ac.uk/~csmora/CSCM67/lab%203-%20prefix%20sum.pdf
*/

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
	delete[] resultArr;
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
