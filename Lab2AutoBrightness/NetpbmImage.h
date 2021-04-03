#pragma once
#include "NetpbmFormat.h"

class NetpbmImage
{
private:
	NetpbmFormat format;
	int width, height;
	int byteSize;
	unsigned char* bytes;
	NetpbmImage(NetpbmFormat format, int width, int height, int byteSize, unsigned char* bytes);

public:
	static NetpbmImage* Read(char* filename);
	void AutoBrightness();
	void Write(char* filename);
	~NetpbmImage();
};
