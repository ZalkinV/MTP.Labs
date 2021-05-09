#pragma once

#include <stdio.h>
#include "TypeDefs.h"

mtype** multiply(mtype** matrixA, mtype** matrixB, int rowsCount, int colsCount, int elsCount);
mtype* multiply(mtype* matrixA, mtype* matrixB, int rowsCount, int colsCount, int elsCount);
mtype* convertTo1D(mtype** matrix, int rowsCount, int colsCount);

void readMatrices(
	char* fileName,
	mtype** firstMatrix, mtype** secondMatrix,
	size_t* firstRowsCount, size_t* colsRowsCount, size_t* secondColsCount);
mtype* readMatrix(FILE* file, const size_t rowsCount, const size_t colsCount);

void printMatrix(mtype* matrix, int rowsCount, int colsCount);
void fprintMatrix(char* fileName, mtype* matrix, int rowsCount, int colsCount);
