#include "RecursiveDeterminant.h"

using namespace std;

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