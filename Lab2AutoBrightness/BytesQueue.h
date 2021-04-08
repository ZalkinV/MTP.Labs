#pragma once
#include <queue>
#include "TypeDefs.h"

using namespace std;

class BytesQueue
{
public:
	int maxSize;

	BytesQueue(int maxSize);

	void push(byte value);
	byte topMin();
	byte topMax();

private:
	priority_queue<byte> minBytes;
	priority_queue<byte, vector<byte>, greater<byte>> maxBytes;
};