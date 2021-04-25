#define CL_TARGET_OPENCL_VERSION 120

#include <CL/opencl.h>

#include "OpenCLOperations.h"
#include "MatrixOperations.h"
#include <vector>


void runSumKernel(cl_context context, cl_device_id deviceId, cl_command_queue queue)
{
	const char* kernelName = "arraySum";
	cl_program program = getProgram(context, "Sum.ocl");
	int buildStatus = buildProgram(program, deviceId);

	cl_int err = 0;
	size_t groupSize = 2;
	size_t arraySize = 2;
	int* arrayA = new int[] { 1, 2 };
	int* arrayB = new int[] { 3, 4 };
	int* arrayC = new int[arraySize];

	size_t bufferSize = arraySize * sizeof(int);
	cl_mem bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY, bufferSize, NULL, &err);
	cl_mem bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY, bufferSize, NULL, &err);
	cl_mem bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, bufferSize, NULL, &err);


	cl_kernel kernel = clCreateKernel(program, kernelName, &err);

	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufferA);
	err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferB);
	err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufferC);

	err = clEnqueueWriteBuffer(queue, bufferA, false, 0, bufferSize, arrayA, NULL, NULL, NULL);
	err = clEnqueueWriteBuffer(queue, bufferB, false, 0, bufferSize, arrayB, NULL, NULL, NULL);

	cl_uint workDim = 1;
	size_t globalWorkSize = arraySize;
	cl_event kernelStartEvent;
	err = clEnqueueNDRangeKernel(queue, kernel, workDim, NULL, &globalWorkSize, NULL, NULL, NULL, &kernelStartEvent);

	err = clEnqueueReadBuffer(queue, bufferC, true, 0, bufferSize, arrayC, NULL, NULL, NULL);

	clReleaseMemObject(bufferA);
	clReleaseMemObject(bufferB);
	clReleaseMemObject(bufferC);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	for (int i = 0; i < arraySize; i++)
	{
		printf("%i ", arrayC[i]);
	}

	delete[] arrayA;
	delete[] arrayB;
	delete[] arrayC;

	int kernelExecTime = getElapsedTimeUs(kernelStartEvent);
	printf("Kernel execution time: %i us\n", kernelExecTime);
}

void runMulKernel(cl_context context, cl_device_id deviceId, cl_command_queue queue)
{
	int rowsCount = 4;
	int colsCount = 2;
	int elsCount = 3;
	int** matrixA2D = new int* [rowsCount]
	{
		new int[] {1, 2, 3},
		new int[] {4, 5, 6},
		new int[] {7, 8, 9},
		new int[] {1, 5, 9},
	};
	int** matrixB2D = new int* [elsCount]
	{
		new int[] {1, 2},
		new int[] {3, 4},
		new int[] {5, 6},
	};

	int* matrixA = convertTo1D(matrixA2D, rowsCount, elsCount);
	int* matrixB = convertTo1D(matrixB2D, elsCount, colsCount);

	int* matrixCorrectResult = multiply(matrixA, matrixB, rowsCount, colsCount, elsCount);


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
	size_t globalWorkSize = matrixCSize;
	cl_event kernelStartEvent;
	err = clEnqueueNDRangeKernel(queue, kernel, workDim, NULL, &globalWorkSize, NULL, NULL, NULL, &kernelStartEvent);


	int* matrixC = new int[matrixCSize];
	err = clEnqueueReadBuffer(queue, bufferC, true, NULL, bufferCSize, matrixC, NULL, NULL, NULL);

	clReleaseMemObject(bufferA);
	clReleaseMemObject(bufferB);
	clReleaseMemObject(bufferC);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	printMatrix(matrixC, rowsCount, colsCount);

	delete[] matrixA;
	delete[] matrixB;
	delete[] matrixC;

	int kernelExecTime = getElapsedTimeUs(kernelStartEvent);
	printf("Kernel execution time: %i us\n", kernelExecTime);
}

int main()
{
	cl_device_id deviceId = getDeviceId();

	cl_context context = clCreateContext(NULL, 1, &deviceId, NULL, NULL, NULL);

	cl_command_queue_properties queueProperties = CL_QUEUE_PROFILING_ENABLE;
	cl_command_queue queue = clCreateCommandQueue(context, deviceId, queueProperties, NULL);

	//runSumKernel(context, deviceId, queue);
	runMulKernel(context, deviceId, queue);
	
	return 0;
}
