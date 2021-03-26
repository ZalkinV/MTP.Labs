#include "Source.h"
#include <string>

using namespace std;

void onThreadsCount()
{
	int minThreadsCount = 1;
	int maxThreadsCount = 16;

	int argc = 3;
	char** argv = new char*[argc];
	argv[1] = (char*)"matrix1024.txt";
	argv[2] = new char[3];
	
	printf("%s\n", argv[1]);
	printf("threadsCount, time (ms)\n");
	for (int i = minThreadsCount; i <= maxThreadsCount; i++)
	{
		strcpy_s(argv[2], 3, to_string(i).c_str());

		int threadsCount = 0;
		float measuredMs = 0;
		matrixDeterminant(argc, argv, threadsCount, measuredMs);

		printf("%i, %f\n", threadsCount, measuredMs);
	}
}

void onOmpOnOff()
{
	int argc = 3;

	printf("threadsCount, time (ms)\n");
	for (int size = 4; size <= 1024; size *= 2)
	{
		string filename = "matrix" + to_string(size) + ".txt";
		char** argv = new char* [argc];
		argv[1] = (char*)filename.c_str();

		printf("%s\n", argv[1]);
		for (int i = -1; i <= 1; i += 2)
		{
			argv[2] = new char[3];
			strcpy_s(argv[2], 3, to_string(i).c_str());

			int threadsCount = 0;
			float measuredMs = 0;
			matrixDeterminant(argc, argv, threadsCount, measuredMs);

			printf("%i, %f\n", threadsCount, measuredMs);
		}
	}
}
