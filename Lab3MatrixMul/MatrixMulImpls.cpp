#define CL_TARGET_OPENCL_VERSION 120

#include "MatrixMulImpls.h"
#include "Timer.h"


mtype* runMulKernel(
	cl_uint deviceIndex,
	mtype* firstMatrix, mtype* secondMatrix,
	int firstRowsCount, int colsRowsCount, int secondColsCount,
	int implementationNumber,
	float* kernelExecTime,
	float* fullElapsedTime)
{
	cl_device_id deviceId = getDeviceId(deviceIndex);
	printDeviceInfo(deviceId);

	cl_context context = clCreateContext(NULL, 1, &deviceId, NULL, NULL, NULL);

	cl_command_queue_properties queueProperties = CL_QUEUE_PROFILING_ENABLE;
	cl_command_queue queue = clCreateCommandQueue(context, deviceId, queueProperties, NULL);


	mtype* resultMatrix = NULL;
	switch (implementationNumber)
	{
	case 1:
		resultMatrix = runFirstImplementation(
			context, deviceId, queue,
			firstMatrix, secondMatrix,
			firstRowsCount, colsRowsCount, secondColsCount,
			kernelExecTime, fullElapsedTime);
		break;
	default:
		break;
	}

	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	clReleaseDevice(deviceId);

	return resultMatrix;
}

mtype* runFirstImplementation(
	cl_context context, cl_device_id deviceId, cl_command_queue queue,
	mtype* firstMatrix, mtype* secondMatrix,
	size_t firstRowsCount, size_t colsRowsCount, size_t secondColsCount,
	float* kernelExecTime, float* fullElapsedTime)
{
	const char* kernelName = "matrixMul";
	cl_program program = getProgram(context, "Mul.ocl");
	int buildStatus = buildProgram(program, deviceId);

	cl_int err = 0;

	cl_kernel kernel = clCreateKernel(program, kernelName, &err);

	Timer timer;
	timer.start();

	size_t firstBufferSize = firstRowsCount * colsRowsCount * sizeof(mtype);
	cl_mem firstBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, firstBufferSize, NULL, &err);
	err = clEnqueueWriteBuffer(queue, firstBuffer, false, 0, firstBufferSize, firstMatrix, NULL, NULL, NULL);

	size_t secondBufferSize = colsRowsCount * secondColsCount * sizeof(mtype);
	cl_mem secondBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, secondBufferSize, NULL, &err);
	err = clEnqueueWriteBuffer(queue, secondBuffer, false, 0, secondBufferSize, secondMatrix, NULL, NULL, NULL);

	size_t resultMatrixSize = firstRowsCount * secondColsCount;
	size_t resultBufferSize = resultMatrixSize * sizeof(mtype);
	cl_mem resultBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, resultBufferSize, NULL, &err);

	cl_uint iArg = 0;
	err = clSetKernelArg(kernel, iArg++, sizeof(cl_mem), &firstBuffer);
	err = clSetKernelArg(kernel, iArg++, sizeof(cl_mem), &secondBuffer);
	err = clSetKernelArg(kernel, iArg++, sizeof(size_t), &colsRowsCount);
	err = clSetKernelArg(kernel, iArg++, sizeof(size_t), &secondColsCount);
	err = clSetKernelArg(kernel, iArg++, sizeof(cl_mem), &resultBuffer);


	cl_uint workDim = 2;
	size_t* globalWorkSize = new size_t[]{ firstRowsCount, secondColsCount };
	cl_event kernelStartEvent;
	err = clEnqueueNDRangeKernel(queue, kernel, workDim, NULL, globalWorkSize, NULL, NULL, NULL, &kernelStartEvent);


	mtype* resultMatrix = new mtype[resultMatrixSize];
	err = clEnqueueReadBuffer(queue, resultBuffer, true, NULL, resultBufferSize, resultMatrix, NULL, NULL, NULL);

	timer.stop();
	*fullElapsedTime = timer.getMs();
	*kernelExecTime = getElapsedTimeMs(kernelStartEvent);

	clReleaseEvent(kernelStartEvent);
	clReleaseMemObject(firstBuffer);
	clReleaseMemObject(secondBuffer);
	clReleaseMemObject(resultBuffer);
	clReleaseProgram(program);

	return resultMatrix;
}
