#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>

#include <CL/opencl.h>


cl_device_id getDeviceId()
{
	cl_uint platformCount = 0;
	clGetPlatformIDs(0, NULL, &platformCount);
	cl_platform_id* platformsIds = new cl_platform_id[platformCount];
	clGetPlatformIDs(platformCount, platformsIds, NULL);

	cl_device_id defaultDeviceId = NULL;
	for (int iPlatform = 0; iPlatform < platformCount; iPlatform++)
	{
		cl_platform_id platformId = platformsIds[iPlatform];

		cl_uint devicesCount = 0;
		clGetDeviceIDs(platformId, CL_DEVICE_TYPE_GPU, 0, NULL, &devicesCount);
		cl_device_id* devicesIds = new cl_device_id[devicesCount];
		clGetDeviceIDs(platformId, CL_DEVICE_TYPE_GPU, devicesCount, devicesIds, NULL);

		for (int iDevice = 0; iDevice < devicesCount; iDevice++)
		{
			cl_device_id deviceId = devicesIds[iDevice];

			if (defaultDeviceId == NULL)
				defaultDeviceId = deviceId;

			int isIntegrated = 0;
			clGetDeviceInfo(deviceId, CL_DEVICE_HOST_UNIFIED_MEMORY, sizeof(int), &isIntegrated, NULL);

			if (!isIntegrated)
				return deviceId;
		}
	}

	return defaultDeviceId;
}

cl_program getProgram(cl_context context)
{
	FILE* file = fopen("Kernel.ocl", "rb");
	if (file == NULL)
		return NULL;

	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	char** sourceCode = new char*[1];
	sourceCode[0] = new char[fileSize];
	fread(sourceCode[0], sizeof(char), fileSize, file);
	
	fclose(file);

	cl_program program = clCreateProgramWithSource(context, 1, (const char**)sourceCode, &fileSize, NULL);

	delete[] sourceCode[0];
	delete[] sourceCode;

	return program;
}

int buildProgram(cl_program program, cl_device_id deviceId)
{
	cl_int compilationErr = clBuildProgram(program, 1, &deviceId, "", NULL, NULL);
	
	if (compilationErr != 0)
	{
		size_t buildLogSize;
		clGetProgramBuildInfo(program, deviceId, CL_PROGRAM_BUILD_LOG, NULL, NULL, &buildLogSize);
		char* buildLog = new char[buildLogSize];
		clGetProgramBuildInfo(program, deviceId, CL_PROGRAM_BUILD_LOG, buildLogSize, buildLog, NULL);
		printf("%s", buildLog);
	}

	return compilationErr;
}

int main()
{
	cl_device_id deviceId = getDeviceId();

	cl_context context = clCreateContext(NULL, 1, &deviceId, NULL, NULL, NULL);

	cl_queue_properties queueProperties[] = { CL_QUEUE_PROFILING_ENABLE };
	cl_command_queue queue = clCreateCommandQueueWithProperties(context, deviceId, queueProperties, NULL);
	
	cl_program program = getProgram(context);
	int buildStatus = buildProgram(program, deviceId);
	if (buildStatus != 0)
		return buildStatus;


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


	cl_kernel kernel = clCreateKernel(program, "sum", &err);

	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufferA);
	err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferB);
	err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufferC);

	err = clEnqueueWriteBuffer(queue, bufferA, false, 0, bufferSize, arrayA, NULL, NULL, NULL);
	err = clEnqueueWriteBuffer(queue, bufferB, false, 0, bufferSize, arrayB, NULL, NULL, NULL);
	
	cl_uint workDim = 1;
	size_t globalWorkSize = 1;
	err = clEnqueueNDRangeKernel(queue, kernel, workDim, NULL, &globalWorkSize, NULL, NULL, NULL, NULL);
	
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
	
	return 0;
}
