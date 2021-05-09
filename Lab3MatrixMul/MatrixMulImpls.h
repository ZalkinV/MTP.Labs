#pragma once

#include "OpenCLOperations.h"
#include "MatrixOperations.h"


mtype* runMulKernel(
	cl_uint deviceIndex,
	mtype* firstMatrix, mtype* secondMatrix,
	size_t firstRowsCount, size_t colsRowsCount, size_t secondColsCount,
	int implementationNumber,
	float* kernelExecTime, float* fullElapsedTime);

mtype* runImplementation(
	const char* kernelName, const size_t* localWorkSize,
	cl_context context, cl_device_id deviceId, cl_command_queue queue,
	mtype* firstMatrix, mtype* secondMatrix,
	size_t firstRowsCount, size_t colsRowsCount, size_t secondColsCount,
	float* kernelExecTime, float* fullElapsedTime);

size_t roundToNextDivisible(size_t value, size_t divider);
