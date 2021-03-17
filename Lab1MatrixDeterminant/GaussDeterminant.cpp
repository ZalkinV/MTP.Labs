#include "GaussDeterminant.h"

using namespace std;

/*
Useful links:
https://www.youtube.com/watch?v=QQ89r0P0zp8
https://pro-prof.com/forums/topic/cplusplus_matrix-triangulation
http://blog.kislenko.net/show.php?id=998
https://www.cyberforum.ru/cpp-beginners/thread2379481.html
https://e-maxx.ru/algo/determinant_gauss
*/

float calcGaussDeterminant(vector<vector<float>>& matrix)
{
	int size = matrix.size();
	
	float determinant = 1;
	int rowsSwappingCount = 0;
	for (int iDiag = 0; iDiag < size; iDiag++)
	{
		if (swapWithFirstNotZeroDiagElRow(matrix, iDiag))
			rowsSwappingCount++;

		float diagEl = matrix[iDiag][iDiag];
		for (int iRow = iDiag + 1; iRow < size; iRow++)
		{
			float toBeZeroEl = matrix[iRow][iDiag];
			float zerofierCoef = toBeZeroEl / diagEl;

			for (int iCol = iDiag; iCol < size; iCol++)
			{
				float zerofierEl = matrix[iDiag][iCol];
				matrix[iRow][iCol] -= zerofierCoef * zerofierEl;
			}
		}

		determinant *= diagEl;
	}

	if (rowsSwappingCount % 2 != 0)
		determinant *= -1;

	return determinant;
}

bool swapWithFirstNotZeroDiagElRow(vector<vector<float>>& matrix, int iDiag)
{
	bool isSwapped = false;
	if (matrix[iDiag][iDiag] == 0)
	{
		int size = matrix.size();
		for (int iRow = iDiag + 1; iRow < size; iRow++)
		{
			if (matrix[iRow][iDiag] != 0)
			{
				swap(matrix[iDiag], matrix[iRow]);
				isSwapped = true;
				break;
			}
		}

	}

	return isSwapped;
}
