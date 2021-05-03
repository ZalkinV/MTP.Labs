#define CL_TARGET_OPENCL_VERSION 120

#include <exception>

#include "MatrixMulImpls.h"
#include <string>

using namespace std;

int getDeviceIndex(char* str)
{
	int parsedValue = stoi(str);

	if (parsedValue < 0 || MAX_DEVICES_COUNT <= parsedValue)
		throw runtime_error("deviceIndex cannot be less than 0 and inclusively greater than " + to_string(MAX_DEVICES_COUNT));

	return parsedValue;
}

int getImplementationNumber(char* str)
{
	int parsedValue = stoi(str);

	if (parsedValue < 1 || 3 < parsedValue)
		throw runtime_error("Implementation number cannot be less than 1 and greater than 3");

	return parsedValue;
}

int main(int argc, char* argv[])
{
	if (argc < 5)
		throw exception("Wrong count of arguments");

	int deviceIndex = getDeviceIndex(argv[1]);
	char* inputFilename = argv[2];
	char* outputFilename = argv[3];
	int implementationNumber = getImplementationNumber(argv[4]);

	cl_device_id deviceId = getDeviceId(deviceIndex);

	cl_context context = clCreateContext(NULL, 1, &deviceId, NULL, NULL, NULL);

	cl_command_queue_properties queueProperties = CL_QUEUE_PROFILING_ENABLE;
	cl_command_queue queue = clCreateCommandQueue(context, deviceId, queueProperties, NULL);

	runMulKernel(context, deviceId, queue);

	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	clReleaseDevice(deviceId);
	
	return 0;
}
