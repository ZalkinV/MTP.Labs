#include <exception>
#include <string>

#include "NetpbmImage.h"

using namespace std;

NetpbmImage::NetpbmImage(NetpbmFormat format, int width, int height, int byteSize, unsigned char* bytes)
{
	if (format != NetpbmFormat::P5 && format != NetpbmFormat::P6)
		throw exception("Wrong Netpbm file format");

	this->format = format;
	this->width = width;
	this->height = height;
	this->byteSize = byteSize;
	this->bytes = bytes;
}

NetpbmImage NetpbmImage::Read(char* filename)
{
	FILE* file = fopen(filename, "rb");
	if (file == NULL)
		throw exception("Cannot open input file");

	int err = 0;
	int format = 0, width = 0, height = 0, byteSize = 0;
	err = fscanf(file, "P%i\n%i %i\n%i\n", &format, &width, &height, &byteSize);
	NetpbmFormat netpbmFormat = (NetpbmFormat)format;
	
	int bytesCount = width * height;
	unsigned char* bytesBuffer = new unsigned char[bytesCount];
	fread(bytesBuffer, sizeof(unsigned char), bytesCount, file);

	NetpbmImage image = NetpbmImage(netpbmFormat, width, height, byteSize, bytesBuffer);

	fclose(file);

	return image;
}

void NetpbmImage::AutoBrightness()
{
}

void NetpbmImage::Write(char* filename)
{
}

NetpbmImage::~NetpbmImage()
{
	delete[] this->bytes;
}
