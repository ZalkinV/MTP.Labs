#pragma once
#include "TypeDefs.h"

void measureTime();
size_t* getSizesPowerOfTwo(size_t len);
void checkCorrectness(mtype* actual, mtype* firstMatrix, mtype* secondMatrix, size_t rowsCount, size_t colsRowsCount, size_t colsCount);
