#include "BytesQueue.h"

BytesQueue::BytesQueue(int maxSize)
{
	this->maxSize = maxSize;
}

void BytesQueue::push(byte value)
{
	byte greatestMin = this->topMin();
	if (value < greatestMin)
	{
		if (this->minBytes.size() >= maxSize)
			this->minBytes.pop();
		this->minBytes.push(value);
	}

	byte smallestMax = this->topMax();
	if (value > smallestMax)
	{
		if (this->maxBytes.size() >= maxSize)
			this->maxBytes.pop();
		this->maxBytes.push(value);
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
