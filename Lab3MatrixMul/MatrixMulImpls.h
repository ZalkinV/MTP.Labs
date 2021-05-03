#pragma once

#include <CL/opencl.h>

#include "OpenCLOperations.h"
#include "MatrixOperations.h"


mtype* runMulKernel(
	cl_uint deviceIndex,
	mtype* firstMatrix, mtype* secondMatrix,
	int firstRowsCount, int colsRowsCount, int secondColsCount,
	int implementationNumber,
	float* kernelExecTime,
	float* fullElapsedTime);
