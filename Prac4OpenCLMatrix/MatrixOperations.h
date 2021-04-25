#pragma once

int** multiply(int** matrixA, int** matrixB, int rowsCount, int colsCount, int elsCount);
int* multiply(int* matrixA, int* matrixB, int rowsCount, int colsCount, int elsCount);
int* convertTo1D(int** matrix, int rowsCount, int colsCount);
void printMatrix(int* matrix, int rowsCount, int colsCount);
