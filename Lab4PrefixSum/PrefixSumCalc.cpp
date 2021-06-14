#define CL_TARGET_OPENCL_VERSION 120
#define _CRT_SECURE_NO_WARNINGS

#include "PrefixSumCalc.h"
#include "Timer.h"
#include "ArrayOperations.h"

using namespace std;


float* calcPrefixSum(
	cl_uint deviceIndex,
	float* arr,
	size_t arrLength,
	size_t localGroupSize,
	size_t chunksCount,
	float* kernelExecTime,
	float* fullElapsedTime)
{
	cl_device_id deviceId = getDeviceId(deviceIndex);
	printDeviceInfo(deviceId);

	cl_int err = 0;
	cl_context context = clCreateContext(NULL, 1, &deviceId, NULL, NULL, &err); tryThrowErr(err);

	cl_command_queue_properties queueProperties = CL_QUEUE_PROFILING_ENABLE;
	cl_command_queue queue = clCreateCommandQueue(context, deviceId, queueProperties, &err); tryThrowErr(err);


	size_t chunkSize = roundToNextDivisible(arrLength, chunksCount) / chunksCount;
	
	size_t globalWorkSizeX = localGroupSize * chunksCount;
	size_t* globalWorkSize = new size_t[] { globalWorkSizeX };
	size_t* localWorkSize = new size_t[] { localGroupSize };

	cl_program program = getProgram(context, "PrefSum.ocl");
	err = buildProgram(program, deviceId); tryThrowErr(err);


	char* kernelName = new char[32];
	strcpy(kernelName, "prefSumStageOne");
	cl_kernel kernel = clCreateKernel(program, kernelName, &err); tryThrowErr(err);

	Timer timer;
	timer.start();

	// Start stage 1
	size_t arrBufferSize = arrLength * sizeof(float);
	cl_mem arrBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, arrBufferSize, NULL, &err); tryThrowErr(err);
	err = clEnqueueWriteBuffer(queue, arrBuffer, false, 0, arrBufferSize, arr, NULL, NULL, NULL); tryThrowErr(err);

	size_t stage1ResultBufferSize = arrLength * sizeof(float);
	cl_mem stage1ResultBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, stage1ResultBufferSize, NULL, &err); tryThrowErr(err);

	cl_uint iArg = 0;
	err = clSetKernelArg(kernel, iArg++, sizeof(cl_mem), &arrBuffer); tryThrowErr(err);
	err = clSetKernelArg(kernel, iArg++, sizeof(int), &arrLength); tryThrowErr(err);
	err = clSetKernelArg(kernel, iArg++, sizeof(int), &chunkSize); tryThrowErr(err);
	err = clSetKernelArg(kernel, iArg++, sizeof(cl_mem), &stage1ResultBuffer); tryThrowErr(err);


	cl_event kernelStartEvent;
	cl_uint workDim = 1;
	err = clEnqueueNDRangeKernel(queue, kernel, workDim, NULL, globalWorkSize, localWorkSize, NULL, NULL, &kernelStartEvent); tryThrowErr(err);


	float* stage1Result = new float[stage1ResultBufferSize];
	err = clEnqueueReadBuffer(queue, stage1ResultBuffer, true, NULL, stage1ResultBufferSize, stage1Result, NULL, NULL, NULL); tryThrowErr(err);
	
	printArray(stage1Result, arrLength);

	// Finish stage 1

	timer.stop();
	*fullElapsedTime = timer.getMs();
	*kernelExecTime = getElapsedTimeMs(kernelStartEvent);

	err = clReleaseEvent(kernelStartEvent); tryThrowErr(err);
	err = clReleaseMemObject(arrBuffer); tryThrowErr(err);
	err = clReleaseMemObject(stage1ResultBuffer); tryThrowErr(err);
	err = clReleaseProgram(program); tryThrowErr(err);

	err = clReleaseCommandQueue(queue); tryThrowErr(err);
	err = clReleaseContext(context); tryThrowErr(err);
	err = clReleaseDevice(deviceId); tryThrowErr(err);

	return stage1Result;
}

float* calcPrefixSumSequential(const float* arr, const size_t length)
{
	float* result = new float[length];

	result[0] = arr[0];
	for (size_t i = 1; i < length; i++)
	{
		result[i] = result[i - 1] + arr[i];
	}

	return result;
}

size_t roundToNextDivisible(size_t value, size_t divider)
{
	if (value <= divider)
		return divider;

	if (value % divider == 0)
		return value;

	size_t lowDivideResult = value / divider;
	size_t highClosestDivisable = (lowDivideResult + 1) * divider;

	return highClosestDivisable;
}

