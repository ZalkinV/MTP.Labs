#include <string>
#include <stdexcept>

#include "ErrorThrower.h"

using namespace std;


void tryThrowErr(int errcode)
{
	if (errcode != 0)
		throw runtime_error("OpenCL error code: " + to_string(errcode));
}
