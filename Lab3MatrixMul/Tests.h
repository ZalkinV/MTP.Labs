#pragma once
#include "TypeDefs.h"

void startTests();
void runTest(int implNumber, mtype* firstMatrix, mtype* secondMatrix, size_t rowsCount, size_t colsRowsCount, size_t colsCount);
void testOnDifferentSizes();

mtype* createRandMatrix(size_t rowsCount, size_t colsCount);
mtype* createSequentialMatrix(size_t rowsCount, size_t colsCount);

bool equals(mtype* firstMatrix, mtype* secondMatrix, size_t rowsCount, size_t colsCount);
void printWrongMatricesInfo(mtype* expected, mtype* actual, mtype* first, mtype* second, size_t rowsCount, size_t colsRowsCount, size_t colsCount);
