#include<exception>
#include<string>
#include<vector>
#include<set>
#include<iostream>
#include<fstream>

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

int getSign(int row, int col)
{
	int sum = col;
	int sign = sum % 2 == 0 ? 1 : -1;
	return sign;
}

float calcDeterminant(vector<vector<float>>& matrix, int elRow, set<int> colsToSkip)
{
	int size = matrix.size();
	float determinant = 0;
	
	int newRow = elRow + 1;

	int sign = 1;
	for (int elCol = 0; elCol < size; elCol++)
	{
		if (colsToSkip.count(elCol) != 0)
			continue;

		float element = matrix[elRow][elCol];
		
		float minor;
		if (newRow != size)
		{
			set<int> newColsToSkip = colsToSkip;
			newColsToSkip.insert(elCol);

			minor = calcDeterminant(matrix, newRow, newColsToSkip);
		}
		else
		{
			return element;
		}
		
		float algAddition = sign * minor;

		float mult = element * algAddition;
		determinant += mult;

		sign = -sign;
	}

	return determinant;
}

float calcDeterminant(vector<vector<float>>& matrix)
{
	set<int> colsToSkip;
	float determinant = calcDeterminant(matrix, 0, colsToSkip);
	return determinant;
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
	float determinant = calcDeterminant(matrix);
	printf("Determinant: %g\n", determinant);
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
