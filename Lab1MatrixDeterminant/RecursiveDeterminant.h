#pragma once
#include <vector>
#include <set>

using namespace std;

float calcDeterminant(vector<vector<float>>& matrix, int elRow, set<int> colsToSkip);
float calcDeterminant(vector<vector<float>>& matrix);
