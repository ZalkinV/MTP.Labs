#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <stdexcept>

#include "ArrayOperations.h"

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

	
	fprintArray(outputFileName, inputArr, arrLength);


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
