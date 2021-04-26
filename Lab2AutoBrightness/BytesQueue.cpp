#include "BytesQueue.h"

BytesQueue::BytesQueue(int maxSize)
{
	int sizeOfSingleQueue = maxSize / 2;
	this->maxSize = sizeOfSingleQueue;
}

void BytesQueue::push(byte value)
{
	if (this->minBytes.size() < this->maxSize)
	{
#pragma omp critical
		{
			this->minBytes.push(value);
		}
	}
	else
	{
		byte greatestMin = this->topMin();
		if (value < greatestMin)
		{
#pragma omp critical
			{
				this->minBytes.pop();
				this->minBytes.push(value);
			}
		}
	}

	if (this->maxBytes.size() < this->maxSize)
	{
#pragma omp critical
		{
			this->maxBytes.push(value);
		}
	}
	else
	{
		byte smallestMax = this->topMax();
		if (value > smallestMax)
		{
#pragma omp critical
			{
				this->maxBytes.pop();
				this->maxBytes.push(value);
			}
		}
	}
}

byte BytesQueue::topMin()
{
	return this->minBytes.top();
}

byte BytesQueue::topMax()
{
	return this->maxBytes.top();
}
