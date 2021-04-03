#pragma once
#include "NetpbmFormat.h"

class NetpbmImage
{
private:
	static const char* HEADER_FORMAT;
	NetpbmFormat format;
	int width, height;
	int byteSize;
	int bytesCount;
	unsigned char* bytes;

	NetpbmImage(NetpbmFormat format, int width, int height, int byteSize, unsigned char* bytes);

	static int CalculateBytesCount(NetpbmFormat format, int width, int height);

public:
	static NetpbmImage* Read(char* filename);
	void AutoBrightness();
	void Write(char* filename);
	~NetpbmImage();
};
