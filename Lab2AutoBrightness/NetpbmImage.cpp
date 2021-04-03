#include <exception>
#include <string>

#include "NetpbmImage.h"

using namespace std;

const char* NetpbmImage::HEADER_FORMAT = "P%i\n%i %i\n%i\n";

NetpbmImage::NetpbmImage(NetpbmFormat format, int width, int height, int byteSize, unsigned char* bytes)
{
	if (format != NetpbmFormat::P5 && format != NetpbmFormat::P6)
		throw exception("Wrong Netpbm file format");

	this->format = format;
	this->width = width;
	this->height = height;
	this->byteSize = byteSize;
	this->bytes = bytes;
	this->bytesCount = NetpbmImage::CalculateBytesCount(format, width, height);
}

int NetpbmImage::CalculateBytesCount(NetpbmFormat format, int width, int height)
{
	int bytesCount = width * height;
	if (format == NetpbmFormat::P6)
		bytesCount *= 3;

	return bytesCount;
}

NetpbmImage* NetpbmImage::Read(char* filename)
{
	FILE* file = fopen(filename, "rb");
	if (file == NULL)
		throw exception("Cannot open input file");

	int format = 0, width = 0, height = 0, byteSize = 0;
	int valuesCount = fscanf(file, HEADER_FORMAT, &format, &width, &height, &byteSize);
	if (valuesCount != 4)
		throw exception("Wrong header in the input file");

	NetpbmFormat netpbmFormat = (NetpbmFormat)format;
	
	int bytesCount = NetpbmImage::CalculateBytesCount(netpbmFormat, width, height);
	unsigned char* bytesBuffer = new unsigned char[bytesCount];
	fread(bytesBuffer, sizeof(unsigned char), bytesCount, file);
	
	fclose(file);

	NetpbmImage* image = new NetpbmImage(netpbmFormat, width, height, byteSize, bytesBuffer);

	return image;
}

void NetpbmImage::AutoBrightness()
{
}

void NetpbmImage::Write(char* filename)
{
	FILE* file = fopen(filename, "wb");
	if (file == NULL)
		throw exception("Cannot create output file");

	int valuesCount = fprintf(file, HEADER_FORMAT, this->format, this->width, this->height, this->byteSize);
	
	fwrite(this->bytes, sizeof(unsigned char), this->bytesCount, file);

	fclose(file);
}

NetpbmImage::~NetpbmImage()
{
	delete[] this->bytes;
}
