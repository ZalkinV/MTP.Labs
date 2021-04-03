#include <exception>
#include <string>
#include <omp.h>

using namespace std;
/*
Useful links:
https://docs.google.com/presentation/d/1pH7sGzIy2-_qd6YYeApTmw5vHwxjV9Y2CwvRAQ82EIc/edit
https://stackoverflow.com/questions/596216/formula-to-determine-brightness-of-rgb-color
*/

int getThreadsCount(char* threadsCountStr)
{
	int threadsCount = stoi(threadsCountStr);

	if (threadsCount < -1)
		throw exception("Thread count cannot be less than -1");

	if (threadsCount == 0)
		threadsCount = omp_get_max_threads();

	if (threadsCount != -1)
		omp_set_num_threads(threadsCount);

	return threadsCount;
}

void labTask(int argc, char* argv[])
{
	if (argc < 4)
		throw exception("Wrong count of arguments");

	char* inputFile = argv[1];
	char* outputFile = argv[2];
	int threadsCount = getThreadsCount(argv[3]);
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