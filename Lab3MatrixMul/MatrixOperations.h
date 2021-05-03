#pragma once

#include "TypeDefs.h"

mtype** multiply(mtype** matrixA, mtype** matrixB, int rowsCount, int colsCount, int elsCount);
mtype* multiply(mtype* matrixA, mtype* matrixB, int rowsCount, int colsCount, int elsCount);
mtype* convertTo1D(mtype** matrix, int rowsCount, int colsCount);
void printMatrix(mtype* matrix, int rowsCount, int colsCount);
