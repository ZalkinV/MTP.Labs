#pragma once
#include "NetpbmFormat.h"

typedef unsigned char byte;

class NetpbmImage
{
private:
	static const char* HEADER_FORMAT;
	NetpbmFormat format;
	int width, height;
	int byteSize;
	int bytesCount;
	byte* bytes;

	NetpbmImage(NetpbmFormat format, int width, int height, int byteSize, byte* bytes);

	static int calculateBytesCount(NetpbmFormat format, int width, int height);

public:
	static NetpbmImage* read(char* filename);
	void autoBrightness();
	void write(char* filename);
	~NetpbmImage();
};
