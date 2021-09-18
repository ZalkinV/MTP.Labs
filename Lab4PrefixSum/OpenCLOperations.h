#pragma once
#include <CL/opencl.h>

#include "ErrorThrower.h"


#define MAX_DEVICES_COUNT 32


cl_device_id getDeviceId(cl_uint deviceIndex);

void printDeviceInfo(cl_device_id deviceId);

void fillDevicesByType(
	const cl_platform_id* platformsIds,
	cl_uint platformsCount,
	cl_device_id* devicesIds,
	cl_device_type deviceType,
	cl_bool isIntegrated,
	cl_uint* startIndex);

cl_program getProgram(cl_context context, const char* sourceFilename);

int buildProgram(cl_program program, cl_device_id deviceId, char* options);

float getElapsedTimeMs(cl_event event);
