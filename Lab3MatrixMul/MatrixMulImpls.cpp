#define CL_TARGET_OPENCL_VERSION 120
#define _CRT_SECURE_NO_WARNINGS

#include "MatrixMulImpls.h"
#include "Timer.h"

using namespace std;


mtype* runMulKernel(
	cl_uint deviceIndex,
	mtype* firstMatrix, mtype* secondMatrix,
	size_t firstRowsCount, size_t colsRowsCount, size_t secondColsCount,
	int implementationNumber,
	float* kernelExecTime,
	float* fullElapsedTime)
{
	cl_device_id deviceId = getDeviceId(deviceIndex);
	printDeviceInfo(deviceId);

	cl_int err = 0;
	cl_context context = clCreateContext(NULL, 1, &deviceId, NULL, NULL, &err); tryThrowErr(err);

	cl_command_queue_properties queueProperties = CL_QUEUE_PROFILING_ENABLE;
	cl_command_queue queue = clCreateCommandQueue(context, deviceId, queueProperties, &err); tryThrowErr(err);

	char* kernelName = new char[32];
	size_t* localWorkSize = NULL;
	size_t* globalWorkSize = new size_t[2];
	size_t localWorkRowsColsCount = LOCAL_GROUP_SIZE;
	size_t globalWorkRowsCount = firstRowsCount;
	size_t globalWorkColsCount = secondColsCount;

	switch (implementationNumber)
	{
	case 1:
		strcpy(kernelName, "firstImpl");
		break;
	case 2:
		strcpy(kernelName, "secondImpl");
		localWorkSize = new size_t[2];
		break;
	case 3:
		strcpy(kernelName, "thirdImpl");
		globalWorkRowsCount /= VEC_SIZE;
		globalWorkColsCount /= VEC_SIZE;

		localWorkSize = new size_t[2];
		localWorkRowsColsCount /= VEC_SIZE;
		break;
	default:
		throw runtime_error("Implementation with number '" + to_string(implementationNumber) + "' does not exist");
	}


	if (localWorkSize != NULL)
	{
		globalWorkRowsCount = roundToNextDivisible(globalWorkRowsCount, localWorkRowsColsCount);
		globalWorkColsCount = roundToNextDivisible(globalWorkColsCount, localWorkRowsColsCount);

		localWorkSize[0] = localWorkRowsColsCount;
		localWorkSize[1] = localWorkRowsColsCount;
	}
	globalWorkSize[0] = globalWorkRowsCount;
	globalWorkSize[1] = globalWorkColsCount;


	mtype* resultMatrix = runImplementation(
		kernelName, globalWorkSize, localWorkSize,
		context, deviceId, queue,
		firstMatrix, secondMatrix,
		firstRowsCount, colsRowsCount, secondColsCount,
		kernelExecTime, fullElapsedTime);

	err = clReleaseCommandQueue(queue); tryThrowErr(err);
	err = clReleaseContext(context); tryThrowErr(err);
	err = clReleaseDevice(deviceId); tryThrowErr(err);

	return resultMatrix;
}

mtype* runImplementation(
	const char* kernelName, const size_t* globalWorkSize, const size_t* localWorkSize,
	cl_context context, cl_device_id deviceId, cl_command_queue queue,
	mtype* firstMatrix, mtype* secondMatrix,
	size_t firstRowsCount, size_t colsRowsCount, size_t secondColsCount,
	float* kernelExecTime, float* fullElapsedTime)
{
	cl_int err = 0;

	cl_program program = getProgram(context, "Mul.ocl");
	err = buildProgram(program, deviceId); tryThrowErr(err);


	cl_kernel kernel = clCreateKernel(program, kernelName, &err); tryThrowErr(err);

	Timer timer;
	timer.start();

	size_t firstBufferSize = firstRowsCount * colsRowsCount * sizeof(mtype);
	cl_mem firstBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, firstBufferSize, NULL, &err); tryThrowErr(err);
	err = clEnqueueWriteBuffer(queue, firstBuffer, false, 0, firstBufferSize, firstMatrix, NULL, NULL, NULL); tryThrowErr(err);

	size_t secondBufferSize = colsRowsCount * secondColsCount * sizeof(mtype);
	cl_mem secondBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, secondBufferSize, NULL, &err); tryThrowErr(err);
	err = clEnqueueWriteBuffer(queue, secondBuffer, false, 0, secondBufferSize, secondMatrix, NULL, NULL, NULL); tryThrowErr(err);

	size_t resultMatrixSize = firstRowsCount * secondColsCount;
	size_t resultBufferSize = resultMatrixSize * sizeof(mtype);
	cl_mem resultBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, resultBufferSize, NULL, &err); tryThrowErr(err);

	cl_uint iArg = 0;
	err = clSetKernelArg(kernel, iArg++, sizeof(cl_mem), &firstBuffer); tryThrowErr(err);
	err = clSetKernelArg(kernel, iArg++, sizeof(cl_mem), &secondBuffer); tryThrowErr(err);
	err = clSetKernelArg(kernel, iArg++, sizeof(size_t), &firstRowsCount); tryThrowErr(err);
	err = clSetKernelArg(kernel, iArg++, sizeof(size_t), &colsRowsCount); tryThrowErr(err);
	err = clSetKernelArg(kernel, iArg++, sizeof(size_t), &secondColsCount); tryThrowErr(err);
	err = clSetKernelArg(kernel, iArg++, sizeof(cl_mem), &resultBuffer); tryThrowErr(err);


	cl_event kernelStartEvent;
	cl_uint workDim = 2;
	
	err = clEnqueueNDRangeKernel(queue, kernel, workDim, NULL, globalWorkSize, localWorkSize, NULL, NULL, &kernelStartEvent); tryThrowErr(err);


	mtype* resultMatrix = new mtype[resultMatrixSize];
	err = clEnqueueReadBuffer(queue, resultBuffer, true, NULL, resultBufferSize, resultMatrix, NULL, NULL, NULL); tryThrowErr(err);

	timer.stop();
	*fullElapsedTime = timer.getMs();
	*kernelExecTime = getElapsedTimeMs(kernelStartEvent);

	err = clReleaseEvent(kernelStartEvent); tryThrowErr(err);
	err = clReleaseMemObject(firstBuffer); tryThrowErr(err);
	err = clReleaseMemObject(secondBuffer); tryThrowErr(err);
	err = clReleaseMemObject(resultBuffer); tryThrowErr(err);
	err = clReleaseProgram(program); tryThrowErr(err);

	return resultMatrix;
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
