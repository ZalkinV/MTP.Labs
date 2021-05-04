#define CL_TARGET_OPENCL_VERSION 120
#define _CRT_SECURE_NO_DEPRECATE

#include "OpenCLOperations.h"


cl_device_id getDeviceId(cl_uint deviceIndex)
{
	cl_int err = 0;

	cl_uint platformCount = 0;
	err = clGetPlatformIDs(0, NULL, &platformCount); tryThrowErr(err);
	cl_platform_id* platformsIds = new cl_platform_id[platformCount];
	err = clGetPlatformIDs(platformCount, platformsIds, NULL); tryThrowErr(err);

	cl_uint nextDeviceIndex = 0;
	cl_device_id* devicesIds = new cl_device_id[MAX_DEVICES_COUNT];
	fillDevicesByType(platformsIds, platformCount, devicesIds, CL_DEVICE_TYPE_GPU, false, &nextDeviceIndex);
	fillDevicesByType(platformsIds, platformCount, devicesIds, CL_DEVICE_TYPE_GPU, true, &nextDeviceIndex);
	fillDevicesByType(platformsIds, platformCount, devicesIds, CL_DEVICE_TYPE_CPU, true, &nextDeviceIndex);

	cl_uint selectedDeviceIndex = 0;
	if (deviceIndex < nextDeviceIndex)
		selectedDeviceIndex = deviceIndex;

	cl_device_id selectedDeviceId = devicesIds[selectedDeviceIndex];

	return selectedDeviceId;
}

void printDeviceInfo(cl_device_id deviceId)
{
	cl_int err = 0;

	printf("Selected device:");
	printf("Id='%p'", deviceId);

	size_t vendorNameSize = 0;
	err = clGetDeviceInfo(deviceId, CL_DEVICE_VENDOR, 0, NULL, &vendorNameSize); tryThrowErr(err);
	char* vendorName = new char[vendorNameSize];
	err = clGetDeviceInfo(deviceId, CL_DEVICE_VENDOR, vendorNameSize, vendorName, NULL); tryThrowErr(err);
	printf(" VendorName='%s'", vendorName);
	delete[] vendorName;

	size_t nameSize = 0;
	err = clGetDeviceInfo(deviceId, CL_DEVICE_NAME, 0, NULL, &nameSize); tryThrowErr(err);
	char* name = new char[nameSize];
	err = clGetDeviceInfo(deviceId, CL_DEVICE_NAME, nameSize, name, NULL); tryThrowErr(err);
	printf(" Name='%s'", name);
	delete[] name;

	printf("\n");
}

void fillDevicesByType(
	const cl_platform_id* platformsIds,
	cl_uint platformsCount,
	cl_device_id* devicesIds,
	cl_device_type deviceType,
	cl_bool isIntegrated,
	cl_uint* startIndex)
{
	cl_int err = 0;

	for (size_t iPlatform = 0; iPlatform < platformsCount; iPlatform++)
	{
		cl_platform_id platformId = platformsIds[iPlatform];

		cl_uint devicesCount = 0;
		err = clGetDeviceIDs(platformId, deviceType, 0, NULL, &devicesCount);
		cl_device_id* curPlatformDevicesIds = new cl_device_id[devicesCount];
		err = clGetDeviceIDs(platformId, deviceType, devicesCount, curPlatformDevicesIds, NULL);

		for (cl_uint iDevice = 0; iDevice < devicesCount; iDevice++)
		{
			cl_device_id deviceId = curPlatformDevicesIds[iDevice];

			cl_bool isInteg = false;
			err = clGetDeviceInfo(deviceId, CL_DEVICE_HOST_UNIFIED_MEMORY, sizeof(cl_bool), &isInteg, NULL); tryThrowErr(err);
			if (isInteg == isIntegrated)
			{
				devicesIds[*startIndex] = deviceId;
				(*startIndex)++;
			}
		}
	}
}

cl_program getProgram(cl_context context, const char* sourceFilename)
{
	FILE* file = fopen(sourceFilename, "rb");
	if (file == NULL)
		return NULL;

	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	char** sourceCode = new char* [1];
	sourceCode[0] = new char[fileSize];
	fread(sourceCode[0], sizeof(char), fileSize, file);

	fclose(file);

	cl_int err = 0;
	cl_program program = clCreateProgramWithSource(context, 1, (const char**)sourceCode, &fileSize, &err); tryThrowErr(err);

	delete[] sourceCode[0];
	delete[] sourceCode;

	return program;
}

cl_int buildProgram(cl_program program, cl_device_id deviceId)
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

float getElapsedTimeMs(cl_event event)
{
	cl_int err = 0;

	cl_ulong startTime = 0, endTime = 0;
	err = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &startTime, NULL);
	err = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &endTime, NULL);

	float elapsedTimeNs = (float)(endTime - startTime);
	float elapsedTimeMs =  elapsedTimeNs / 1000 / 1000;

	return elapsedTimeMs;
}
