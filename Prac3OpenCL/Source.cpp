#include <stdio.h>
#include <CL/opencl.h>

// OpenCL configuration instruction: https://www.notion.so/Visual-Studio-OpenCL-cad4d748972b4464bb0c7f2d09b17cc9

void printDeviceInfo(cl_device_id deviceId)
{
	printf("\tID: %p\n", deviceId);

	size_t nameSize = 0;
	clGetDeviceInfo(deviceId, CL_DEVICE_NAME, 0, NULL, &nameSize);
	char* name = new char[nameSize];
	clGetDeviceInfo(deviceId, CL_DEVICE_NAME, nameSize, name, NULL);
	printf("\t\tName: %s\n", name);
	delete[] name;

	cl_device_type type = 0;
	clGetDeviceInfo(deviceId, CL_DEVICE_TYPE, sizeof(cl_device_type), &type, NULL);
	char* typeName = new char[4];
	switch (type)
	{
	case 0:
		typeName = (char*)"DEF";
		break;
	case 2:
		typeName = (char*)"CPU";
		break;
	case 4:
		typeName = (char*)"GPU";
		break;
	case 8:
		typeName = (char*)"ACL";
		break;
	}

	int isIntegrated = 0;
	clGetDeviceInfo(deviceId, CL_DEVICE_HOST_UNIFIED_MEMORY, sizeof(int), &isIntegrated, NULL);
	
	printf("\t\tType: %s %s\n", typeName, isIntegrated ? "Integrated" : "Discrete");
}

void printAvailableDevicesInfos(cl_platform_id platformId)
{
	cl_uint devicesCount = 0;
	clGetDeviceIDs(platformId, CL_DEVICE_TYPE_ALL, 0, NULL, &devicesCount);
	cl_device_id* devicesIds = new cl_device_id[devicesCount];
	clGetDeviceIDs(platformId, CL_DEVICE_TYPE_ALL, devicesCount, devicesIds, NULL);

	printf("\tDevices count: %i\n", devicesCount);
	for (int i = 0; i < devicesCount; i++)
	{
		cl_device_id deviceId = devicesIds[i];
		printDeviceInfo(deviceId);
	}
}

void printPlatformInfo(cl_platform_id platformId)
{
	printf("ID: %p\n", platformId);

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

	printAvailableDevicesInfos(platformId);
}

void printAvailablePlatformsInfos()
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
}

int main()
{
	printAvailablePlatformsInfos();

	return 0;
}
