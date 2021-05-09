#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <stdexcept>

#include "MatrixMulImpls.h"

using namespace std;


int getDeviceIndex(char* str)
{
	int parsedValue = stoi(str);

	if (parsedValue < 0 || MAX_DEVICES_COUNT <= parsedValue)
		throw runtime_error("deviceIndex cannot be less than 0 and inclusively greater than " + to_string(MAX_DEVICES_COUNT));

	return parsedValue;
}

mtype* readMatrix(FILE* file, const size_t rowsCount, const size_t colsCount)
{
	int err = 0;
	size_t elementsCount = rowsCount * colsCount;
	mtype* matrix = new mtype[elementsCount];
	for (size_t i = 0; i < elementsCount; i++)
	{
		err = fscanf(file, "%f", &matrix[i]);
	}

	return matrix;
}

void readMatrices(
	char* fileName,
	mtype** firstMatrix, mtype** secondMatrix,
	size_t* firstRowsCount, size_t* colsRowsCount, size_t* secondColsCount)
{
	FILE* file = fopen(fileName, "r");
	if (file == NULL)
		throw runtime_error("Cannot open file '" + string(fileName) + "'");

	int err = fscanf(file, "%i %i %i", secondColsCount, colsRowsCount, firstRowsCount);
	
	*firstMatrix = readMatrix(file, *firstRowsCount, *colsRowsCount);
	*secondMatrix = readMatrix(file, *colsRowsCount, *secondColsCount);

	fclose(file);
}

void labTask(int argc, char* argv[])
{
	if (argc < 5)
		throw exception("Wrong count of arguments");

	int deviceIndex = getDeviceIndex(argv[1]);
	char* inputFileName = argv[2];
	char* outputFileName = argv[3];
	int implementationNumber = stoi(argv[4]);

	mtype* firstMatrix = NULL; mtype* secondMatrix = NULL;
	size_t firstRowsCount = 0; size_t colsRowsCount = 0; size_t secondColsCount = 0;
	readMatrices(inputFileName, &firstMatrix, &secondMatrix, &firstRowsCount, &colsRowsCount, &secondColsCount);

	float kernelExecTime = 0;
	float fullElapsedTime = 0;
	mtype* resultMatrix = runMulKernel(deviceIndex, firstMatrix, secondMatrix, firstRowsCount, colsRowsCount, secondColsCount, implementationNumber, &kernelExecTime, &fullElapsedTime);
	printf("\nTime: %f\t%f\n", kernelExecTime, fullElapsedTime);
	
	fprintMatrix(outputFileName, resultMatrix, firstRowsCount, secondColsCount);


	delete[] resultMatrix;
	delete[] firstMatrix;
	delete[] secondMatrix;
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
