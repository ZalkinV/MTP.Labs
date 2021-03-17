#include <exception>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include "GaussDeterminant.h"

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

float measureDeterminantCalculationMs(float (*func)(vector<vector<float>>&), vector<vector<float>>& matrix, float& determinant)
{
	chrono::steady_clock::time_point startTime = chrono::high_resolution_clock::now();
	determinant = func(matrix);
	chrono::steady_clock::time_point endTime = chrono::high_resolution_clock::now();;
	chrono::duration<float, milli> milliseconds = endTime - startTime;

	return milliseconds.count();
}

void matrixDeterminant(int argc, char* argv[])
{
	if (argc < 3)
		throw exception("Wrong count of arguments");

	char* filename = argv[1];
	int threadCount = stoi(argv[2]);
	if (threadCount < -1)
		throw exception("Thread count cannot be less than -1");

	vector<vector<float>> matrix = readMatrix(filename);
	vector<vector<float>> test1({ { 1, 2 }, {3, 4} });
	vector<vector<float>> test2({ { 1, 2, 3 }, { 4, 5, 6}, { 7, 8, 9} });

	float determinant = 0;
	float measuredMs = measureDeterminantCalculationMs(calcGaussDeterminant, matrix, determinant);

	printf("Determinant: %g\n", determinant);
	printf("\nTime (%i thread(s)): %f ms\n", 1, measuredMs);
}

int main(int argc, char* argv[])
{
	try
	{
		matrixDeterminant(argc, argv);
	}
	catch (const exception& e)
	{
		fprintf(stderr, e.what());
		return 1;
	}

	return 0;
}
