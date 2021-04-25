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

int main()
{
	const int rowsCount = 4;
	const int colsCount = 2;
	const int elsCount = 3;
	int** matrixA = new int* [rowsCount]
	{
		new int[] {1, 2, 3},
		new int[] {4, 5, 6},
		new int[] {7, 8, 9},
		new int[] {1, 5, 9},
	};
	int** matrixB = new int* [elsCount]
	{
		new int[] {1, 2},
		new int[] {3, 4},
		new int[] {5, 6},
	};

	int* matrixA1D = convertTo1D(matrixA, rowsCount, elsCount);
	int* matrixB1D = convertTo1D(matrixB, elsCount, colsCount);

	int* matrixCorrectResult = multiply(matrixA1D, matrixB1D, rowsCount, colsCount, elsCount);


	cl_device_id deviceId = getDeviceId();

	cl_context context = clCreateContext(NULL, 1, &deviceId, NULL, NULL, NULL);

	cl_queue_properties queueProperties = CL_QUEUE_PROFILING_ENABLE;
#pragma warning(suppress : 4996)
	cl_command_queue queue = clCreateCommandQueue(context, deviceId, queueProperties, NULL);

	runSumKernel(context, deviceId, queue);
	
	return 0;
}
