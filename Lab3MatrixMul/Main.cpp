#define CL_TARGET_OPENCL_VERSION 120

#include <exception>
#include <fstream>
#include <string>

#include "MatrixMulImpls.h"

using namespace std;

int getDeviceIndex(char* str)
{
	int parsedValue = stoi(str);

	if (parsedValue < 0 || MAX_DEVICES_COUNT <= parsedValue)
		throw runtime_error("deviceIndex cannot be less than 0 and inclusively greater than " + to_string(MAX_DEVICES_COUNT));

	return parsedValue;
}

int getImplementationNumber(char* str)
{
	int parsedValue = stoi(str);

	if (parsedValue < 1 || 3 < parsedValue)
		throw runtime_error("Implementation number cannot be less than 1 and greater than 3");

	return parsedValue;
}

void readMatrices(
	char* fileName,
	float* firstMatrix, float* secondMatrix,
	int* firstRowsCount, int* colsRowsCount, int* secondColsCount)
{
	ifstream file(fileName);
	if (!file)
		throw runtime_error("Cannot open file '" + string(fileName) + "'");

	file >> *secondColsCount >> *colsRowsCount >> * firstRowsCount;
	
	int firstMatrixCount = *firstRowsCount * *colsRowsCount;
	firstMatrix = new float[firstMatrixCount];
	for (size_t i = 0; i < firstMatrixCount; i++)
	{
		file >> firstMatrix[i];
	}
	
	int secondMatrixCount = *colsRowsCount * *secondColsCount;
	secondMatrix = new float[secondMatrixCount];
	for (size_t i = 0; i < secondMatrixCount; i++)
	{
		file >> secondMatrix[i];
	}
}

void labTask(int argc, char* argv[])
{
	if (argc < 5)
		throw exception("Wrong count of arguments");

	int deviceIndex = getDeviceIndex(argv[1]);
	char* inputFileName = argv[2];
	char* outputFileName = argv[3];
	int implementationNumber = getImplementationNumber(argv[4]);

	float* firstMatrix = NULL; float* secondMatrix = NULL;
	int firstRowsCount = 0; int colsRowsCount = 0; int secondColsCount = 0;
	readMatrices(inputFileName, firstMatrix, secondMatrix, &firstRowsCount, &colsRowsCount, &secondColsCount);

	cl_device_id deviceId = getDeviceId(deviceIndex);

	cl_context context = clCreateContext(NULL, 1, &deviceId, NULL, NULL, NULL);

	cl_command_queue_properties queueProperties = CL_QUEUE_PROFILING_ENABLE;
	cl_command_queue queue = clCreateCommandQueue(context, deviceId, queueProperties, NULL);

	runMulKernel(context, deviceId, queue);

	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	clReleaseDevice(deviceId);
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
