#pragma once
#include "NetpbmFormat.h"
#include "TypeDefs.h"

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
	static byte scaleColor(byte value, byte min, byte max);

public:
	static NetpbmImage* read(const char* filename);
	void autoBrightness();
	void autoBrightnessST();
	void write(char* filename);
	~NetpbmImage();
};
