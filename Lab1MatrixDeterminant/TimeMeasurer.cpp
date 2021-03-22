#include "Source.h"
#include <string>

using namespace std;

void onThreadsCount()
{
	int minThreadsCount = 1;
	int maxThreadsCount = 16;

	int argc = 3;
	char** argv = new char*[argc];
	argv[1] = (char*)"matrix512.txt";
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
