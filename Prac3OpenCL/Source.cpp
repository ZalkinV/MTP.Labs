#include <stdio.h>
#include <CL/opencl.h>

// OpenCL configuration instruction: https://www.notion.so/Visual-Studio-OpenCL-cad4d748972b4464bb0c7f2d09b17cc9

void printPlatformInfo(cl_platform_id platformId)
{
	printf("ID: %i\n", platformId);

	size_t platfromNameSize = 0;
	clGetPlatformInfo(platformId, CL_PLATFORM_NAME, NULL, NULL, &platfromNameSize);
	char* platformName = new char[platfromNameSize];
	clGetPlatformInfo(platformId, CL_PLATFORM_NAME, platfromNameSize, platformName, NULL);
	printf("\tName: %s\n", platformName);
	delete[] platformName;

	size_t platformVersionSize = 0;
	clGetPlatformInfo(platformId, CL_PLATFORM_VERSION, NULL, NULL, &platformVersionSize);
	char* platformVersion = new char[platformVersionSize];
	clGetPlatformInfo(platformId, CL_PLATFORM_VERSION, platformVersionSize, platformVersion, NULL);
	printf("\tVersion: %s\n", platformVersion);
	delete[] platformVersion;
}

int main()
{
	cl_uint platformCount = 0;
	int err = clGetPlatformIDs(0, NULL, &platformCount);
	cl_platform_id* platformsIds = new cl_platform_id[platformCount];
	err = clGetPlatformIDs(platformCount, platformsIds, NULL);

	printf("OpenCL platforms count: %i\n", platformCount);
	for (int i = 0; i < platformCount; i++)
	{
		cl_platform_id platformId = platformsIds[i];
		printPlatformInfo(platformId);
	}

	delete[] platformsIds;

	return 0;
}
