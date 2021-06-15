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
	size_t globalWorkSize[] = { globalWorkSizeX };
	size_t localWorkSize[] = { localGroupSize };

	cl_program program = getProgram(context, "PrefSum.ocl");
	char* buildOptions = new char[64];
	sprintf(buildOptions, "-D LOCAL_GROUP_SIZE=%i", localGroupSize);
	err = buildProgram(program, deviceId, buildOptions); tryThrowErr(err);


	char* kernelName = new char[32];

	Timer timer;
	timer.start();

	// Start stage 1. Calc sums inside chunks
	strcpy(kernelName, "prefSumStageOne");
	cl_kernel kernel = clCreateKernel(program, kernelName, &err); tryThrowErr(err);

	size_t arrBufferSize = arrLength * sizeof(float);
	cl_mem arrBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, arrBufferSize, NULL, &err); tryThrowErr(err);
	err = clEnqueueWriteBuffer(queue, arrBuffer, false, 0, arrBufferSize, arr, NULL, NULL, NULL); tryThrowErr(err);

	size_t stage1ResultBufferSize = arrLength * sizeof(float);
	cl_mem stage1ResultBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE, stage1ResultBufferSize, NULL, &err); tryThrowErr(err);

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
	*kernelExecTime += getElapsedTimeMs(kernelStartEvent);
	
	printStageArray("1", stage1Result, arrLength);
	// Finish stage 1


	// Start stage 2. Get chunks' last elements and sum them
	strcpy(kernelName, "prefSumStageTwo");
	cl_kernel kernel2 = clCreateKernel(program, kernelName, &err); tryThrowErr(err);

	size_t stage2ResultBufferSize = chunksCount * sizeof(float);
	cl_mem stage2ResultBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, stage2ResultBufferSize, NULL, &err); tryThrowErr(err);

	iArg = 0;
	err = clSetKernelArg(kernel2, iArg++, sizeof(cl_mem), &stage1ResultBuffer); tryThrowErr(err);
	err = clSetKernelArg(kernel2, iArg++, sizeof(int), &arrLength); tryThrowErr(err);
	err = clSetKernelArg(kernel2, iArg++, sizeof(int), &chunkSize); tryThrowErr(err);
	err = clSetKernelArg(kernel2, iArg++, sizeof(cl_mem), &stage2ResultBuffer); tryThrowErr(err);

	size_t globalWorkSizeStage2[] = { 1 };
	size_t localWorkSizeStage2[] = { 1 };
	err = clEnqueueNDRangeKernel(queue, kernel2, workDim, NULL, globalWorkSizeStage2, localWorkSizeStage2, NULL, NULL, &kernelStartEvent); tryThrowErr(err);


	float* stage2Result = new float[stage2ResultBufferSize];
	err = clEnqueueReadBuffer(queue, stage2ResultBuffer, true, NULL, stage2ResultBufferSize, stage2Result, NULL, NULL, NULL); tryThrowErr(err);
	*kernelExecTime += getElapsedTimeMs(kernelStartEvent);

	printStageArray("2", stage2Result, chunksCount);
	// Finish stage 2


	// Start stage 3. Sum chunks last elements to result array
	strcpy(kernelName, "prefSumStageThree");
	cl_kernel kernel3 = clCreateKernel(program, kernelName, &err); tryThrowErr(err);

	iArg = 0;
	err = clSetKernelArg(kernel3, iArg++, sizeof(cl_mem), &stage2ResultBuffer); tryThrowErr(err);
	err = clSetKernelArg(kernel3, iArg++, sizeof(int), &chunkSize); tryThrowErr(err);
	err = clSetKernelArg(kernel3, iArg++, sizeof(int), &arrLength); tryThrowErr(err);
	err = clSetKernelArg(kernel3, iArg++, sizeof(cl_mem), &stage1ResultBuffer); tryThrowErr(err);

	err = clEnqueueNDRangeKernel(queue, kernel3, workDim, NULL, globalWorkSize, localWorkSize, NULL, NULL, &kernelStartEvent); tryThrowErr(err);

	float* stage3Result = new float[arrLength];
	err = clEnqueueReadBuffer(queue, stage1ResultBuffer, true, NULL, stage1ResultBufferSize, stage3Result, NULL, NULL, NULL); tryThrowErr(err);
	*kernelExecTime += getElapsedTimeMs(kernelStartEvent);

	printStageArray("3", stage3Result, arrLength);
	// Finish stage 3

	timer.stop();
	*fullElapsedTime = timer.getMs();

	err = clReleaseEvent(kernelStartEvent); tryThrowErr(err);
	err = clReleaseMemObject(arrBuffer); tryThrowErr(err);
	err = clReleaseMemObject(stage1ResultBuffer); tryThrowErr(err);
	err = clReleaseMemObject(stage2ResultBuffer); tryThrowErr(err);
	err = clReleaseProgram(program); tryThrowErr(err);

	delete[] stage1Result;
	delete[] stage2Result;

	err = clReleaseCommandQueue(queue); tryThrowErr(err);
	err = clReleaseContext(context); tryThrowErr(err);
	err = clReleaseDevice(deviceId); tryThrowErr(err);

	return stage3Result;
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
