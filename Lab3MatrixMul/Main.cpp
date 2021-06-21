#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <stdexcept>

#include "MatrixMulImpls.h"
#include "Tests.h"

using namespace std;

/*
* Useful links:
* Инструкция по настройке проекта в Visual Studio для работы с OpenCL - https://www.notion.so/Visual-Studio-OpenCL-cad4d748972b4464bb0c7f2d09b17cc9
* OpenCL. Как начать - https://habr.com/ru/post/261323/
* OpenCL on Visual Studio : Configuration tutorial for the confused - https://medium.com/@pratikone/opencl-on-visual-studio-configuration-tutorial-for-the-confused-3ec1c2b5f0ca
* Introduction to OpenCL Programming - http://developer.amd.com/wordpress/media/2013/01/Introduction_to_OpenCL_Programming-201005.pdf
* Tutorial: OpenCL SGEMM tuning for Kepler - https://cnugteren.github.io/tutorial/pages/page1.html
* Case study: matrix multiplication - http://www.cmsoft.com.br/opencl-tutorial/case-study-matrix-multiplication/
* Matrix Multiplication with OpenCL - https://www.fz-juelich.de/SharedDocs/Downloads/IAS/JSC/EN/slides/opencl/opencl-08-matmul.pdf?__blob=publicationFile
*/


int getDeviceIndex(char* str)
{
	int parsedValue = stoi(str);

	if (parsedValue < 0 || MAX_DEVICES_COUNT <= parsedValue)
		throw runtime_error("deviceIndex cannot be less than 0 and inclusively greater than " + to_string(MAX_DEVICES_COUNT));

	return parsedValue;
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
#ifdef TEST
		startTests();
#else
		labTask(argc, argv);
#endif
	}
	catch (const exception& e)
	{
		fprintf(stderr, "%s\n", e.what());
		return 1;
	}
	
	return 0;
}
