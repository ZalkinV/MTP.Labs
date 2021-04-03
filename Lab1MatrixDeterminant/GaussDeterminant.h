#pragma once
#include <vector>
#include <omp.h>

using namespace std;

float calcGaussDeterminant(vector<vector<float>>& matrix);
float calcGaussDeterminantMT(vector<vector<float>>& matrix);
bool swapWithFirstNotZeroDiagElRow(vector<vector<float>>& matrix, int iDiag);
