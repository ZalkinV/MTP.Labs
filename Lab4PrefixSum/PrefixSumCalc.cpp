
float* calcPrefixSumSequential(const float* arr, const size_t length)
{
	float* result = new float[length];

	result[0] = arr[0];
	for (size_t i = 1; i < length; i++)
	{
		result[i] = result[i - 1] + arr[i];
	}

	return result;
}
