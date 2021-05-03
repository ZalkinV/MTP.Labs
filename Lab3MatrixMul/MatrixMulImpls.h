#pragma once

#include <CL/opencl.h>

#include "OpenCLOperations.h"
#include "MatrixOperations.h"


mtype* runMulKernel(
	cl_context context, cl_device_id deviceId, cl_command_queue queue,
	mtype* firstMatrix, mtype* secondMatrix,
	int firstRowsCount, int colsRowsCount, int secondColsCount,
	int implementationNumber);
