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
	for (int iDiag = 0; iDiag < size; iDiag++)
	{
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

	return determinant;
}
