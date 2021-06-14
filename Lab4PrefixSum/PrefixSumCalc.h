#pragma once

#include "OpenCLOperations.h"

float* calcPrefixSum(
	cl_uint deviceIndex,
	float* arr,
	size_t arrLength,
	size_t localGroupSize,
	size_t chunksCount,
	float* kernelExecTime,
	float* fullElapsedTime);

float* calcPrefixSumSequential(const float* arr, const size_t length);

size_t roundToNextDivisible(size_t value, size_t divider);
