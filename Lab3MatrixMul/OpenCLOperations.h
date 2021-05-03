#pragma once
#include <iostream>
#include <CL/opencl.h>

cl_device_id getDeviceId();
cl_program getProgram(cl_context context, const char* sourceFilename);
int buildProgram(cl_program program, cl_device_id deviceId);
int getElapsedTimeUs(cl_event event);
