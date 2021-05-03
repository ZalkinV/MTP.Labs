#define CL_TARGET_OPENCL_VERSION 120

#include "MatrixMulImpls.h"


int main(int argc, char* argv[])
{
	cl_device_id deviceId = getDeviceId(0);

	cl_context context = clCreateContext(NULL, 1, &deviceId, NULL, NULL, NULL);

	cl_command_queue_properties queueProperties = CL_QUEUE_PROFILING_ENABLE;
	cl_command_queue queue = clCreateCommandQueue(context, deviceId, queueProperties, NULL);

	runMulKernel(context, deviceId, queue);

	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	clReleaseDevice(deviceId);
	
	return 0;
}
