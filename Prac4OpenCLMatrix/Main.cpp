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

int main()
{
	cl_device_id deviceId = getDeviceId();

	cl_context context = clCreateContext(NULL, 1, &deviceId, NULL, NULL, NULL);

	cl_queue_properties queueProperties[] = { CL_QUEUE_PROFILING_ENABLE };
	cl_command_queue queue = clCreateCommandQueueWithProperties(context, deviceId, queueProperties, NULL);
	


	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	
	return 0;
}
