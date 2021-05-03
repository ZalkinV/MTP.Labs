#pragma once

#include <CL/opencl.h>

#include "OpenCLOperations.h"
#include "MatrixOperations.h"


void runMulKernel(cl_context context, cl_device_id deviceId, cl_command_queue queue);
