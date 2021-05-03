#include "MatrixMulImpls.h"


void runMulKernel(cl_context context, cl_device_id deviceId, cl_command_queue queue)
{
	size_t rowsCount = 4;
	size_t colsCount = 2;
	size_t elsCount = 3;
	mtype** matrixA2D = new mtype* [rowsCount]
	{
		new mtype[] {1, 2, 3},
		new mtype[] {4, 5, 6},
		new mtype[] {7, 8, 9},
		new mtype[] {1, 5, 9},
	};
	mtype** matrixB2D = new mtype* [elsCount]
	{
		new mtype[] {1, 2},
		new mtype[] {3, 4},
		new mtype[] {5, 6},
	};

	mtype* matrixA = convertTo1D(matrixA2D, rowsCount, elsCount);
	mtype* matrixB = convertTo1D(matrixB2D, elsCount, colsCount);

	mtype* matrixCorrectResult = multiply(matrixA, matrixB, rowsCount, colsCount, elsCount);


	const char* kernelName = "matrixMul";
	cl_program program = getProgram(context, "Mul.ocl");
	int buildStatus = buildProgram(program, deviceId);

	cl_int err = 0;

	cl_kernel kernel = clCreateKernel(program, kernelName, &err);

	size_t bufferASize = rowsCount * elsCount * sizeof(int);
	cl_mem bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY, bufferASize, NULL, &err);
	err = clEnqueueWriteBuffer(queue, bufferA, false, 0, bufferASize, matrixA, NULL, NULL, NULL);

	size_t bufferBSize = elsCount * colsCount * sizeof(int);
	cl_mem bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY, bufferBSize, NULL, &err);
	err = clEnqueueWriteBuffer(queue, bufferB, false, 0, bufferBSize, matrixB, NULL, NULL, NULL);

	size_t matrixCSize = rowsCount * colsCount;
	size_t bufferCSize = matrixCSize * sizeof(int);
	cl_mem bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, bufferCSize, NULL, &err);

	cl_uint iArg = 0;
	err = clSetKernelArg(kernel, iArg++, sizeof(cl_mem), &bufferA);
	err = clSetKernelArg(kernel, iArg++, sizeof(cl_mem), &bufferB);
	err = clSetKernelArg(kernel, iArg++, sizeof(int), &colsCount);
	err = clSetKernelArg(kernel, iArg++, sizeof(int), &elsCount);
	err = clSetKernelArg(kernel, iArg++, sizeof(cl_mem), &bufferC);


	cl_uint workDim = 2;
	size_t* globalWorkSize = new size_t[]{ rowsCount, colsCount };
	cl_event kernelStartEvent;
	err = clEnqueueNDRangeKernel(queue, kernel, workDim, NULL, globalWorkSize, NULL, NULL, NULL, &kernelStartEvent);


	mtype* matrixC = new mtype[matrixCSize];
	err = clEnqueueReadBuffer(queue, bufferC, true, NULL, bufferCSize, matrixC, NULL, NULL, NULL);

	clReleaseMemObject(bufferA);
	clReleaseMemObject(bufferB);
	clReleaseMemObject(bufferC);
	clReleaseProgram(program);

	printMatrix(matrixC, rowsCount, colsCount);

	delete[] matrixA;
	delete[] matrixB;
	delete[] matrixC;

	int kernelExecTime = getElapsedTimeUs(kernelStartEvent);
	printf("Kernel execution time: %i us\n", kernelExecTime);
	clReleaseEvent(kernelStartEvent);
}
