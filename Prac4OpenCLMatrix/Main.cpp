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

	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	
	return 0;
}
