#include <exception>
#include <string>
#include <iostream>
#include <fstream>
#include "Timer.h"
#include "GaussDeterminant.h"
#include "TimeMeasurer.h"

using namespace std;

vector<vector<float>> readMatrix(char* filename)
{
	ifstream file(filename);
	if (!file)
		throw exception("File does not exist");

	int matrixSize;
	file >> matrixSize;
	vector<vector<float>> matrix(matrixSize, vector<float>(matrixSize));

	for (int i = 0; i < matrixSize; i++)
	{
		for (int j = 0; j < matrixSize; j++)
		{
			float value;
			file >> value;
			matrix[i][j] = value;
		}
	}
	file.close();

	return matrix;
}

float matrixDeterminant(int argc, char* argv[], int& threadsCount, float& measuredMs)
{
	if (argc < 3)
		throw exception("Wrong count of arguments");

	char* filename = argv[1];
	threadsCount = stoi(argv[2]);

	if (threadsCount < -1)
		throw exception("Thread count cannot be less than -1");

	if (threadsCount == 0)
		threadsCount = omp_get_max_threads();

	if (threadsCount != -1)
		omp_set_num_threads(threadsCount);

	vector<vector<float>> matrix = readMatrix(filename);

	float determinant;
	Timer timer;
	timer.start();
	if (threadsCount == -1)
		determinant = calcGaussDeterminant(matrix);
	else
		determinant = calcGaussDeterminantMT(matrix);
	timer.stop();
	measuredMs = timer.getMs();

	return determinant;
}

void labTask(int argc, char* argv[])
{
	int threadsCount = 0;
	float measuredMs = 0;
	float determinant = matrixDeterminant(argc, argv, threadsCount, measuredMs);

	printf("Determinant: %g\n", determinant);
	printf("\nTime (%i thread(s)): %f ms\n", threadsCount, measuredMs);
}

int main(int argc, char* argv[])
{
	try
	{
		labTask(argc, argv);
		//onThreadsCount();
		//onOmpOnOff();
		//onSchedules();
	}
	catch (const exception& e)
	{
		fprintf(stderr, e.what());
		return 1;
	}

	return 0;
}
