#include<exception>
#include<string>
#include<vector>
#include<iostream>
#include<fstream>

using namespace std;

vector<vector<double>> readMatrix(char* filename)
{
	ifstream file(filename);
	if (!file)
		throw exception("File does not exist");

	int matrixSize;
	file >> matrixSize;
	vector<vector<double>> matrix(matrixSize, vector<double>(matrixSize));

	for (int i = 0; i < matrixSize; i++)
	{
		for (int j = 0; j < matrixSize; j++)
		{
			double value;
			file >> value;
			matrix[i][j] = value;
		}
	}
	file.close();

	return matrix;
}

void matrixDeterminant(int argc, char* argv[])
{
	if (argc < 3)
		throw exception("Wrong count of arguments");

	char* filename = argv[1];
	int threadCount = stoi(argv[2]);
	if (threadCount < -1)
		throw exception("Thread count cannot be less than -1");

	vector<vector<double>> matrix = readMatrix(filename);
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
