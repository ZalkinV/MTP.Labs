#include <exception>
#include <string>

#include "NetpbmImage.h"

using namespace std;

const char* NetpbmImage::HEADER_FORMAT = "P%i\n%i %i\n%i%1\n"; // по стандарту С++ \n значит, что мы считываем один или несколько пробельных символов, поэтому, чтобы не пропускать возможные пробельные символы в начале блока данных, используется такой шаблон заголовка с %1\n, чтобы пропускать только один символ переноса строки

NetpbmImage::NetpbmImage(NetpbmFormat format, int width, int height, int byteSize, byte* bytes)
{
	if (format != NetpbmFormat::P5 && format != NetpbmFormat::P6)
		throw exception("Wrong Netpbm file format");

	this->format = format;
	this->width = width;
	this->height = height;
	this->byteSize = byteSize;
	this->bytes = bytes;
	this->bytesCount = NetpbmImage::calculateBytesCount(format, width, height);
}

int NetpbmImage::calculateBytesCount(NetpbmFormat format, int width, int height)
{
	int bytesCount = width * height;
	if (format == NetpbmFormat::P6)
		bytesCount *= 3;

	return bytesCount;
}

NetpbmImage* NetpbmImage::read(char* filename)
{
	FILE* file = fopen(filename, "rb");
	if (file == NULL)
		throw exception("Cannot open input file");

	int format = 0, width = 0, height = 0, byteSize = 0;
	int valuesCount = fscanf(file, HEADER_FORMAT, &format, &width, &height, &byteSize);
	if (valuesCount != 4)
		throw exception("Wrong header in the input file");

	NetpbmFormat netpbmFormat = (NetpbmFormat)format;
	
	int bytesCount = NetpbmImage::calculateBytesCount(netpbmFormat, width, height);
	byte* bytesBuffer = new byte[bytesCount];
	fread(bytesBuffer, sizeof(byte), bytesCount, file);
	
	fclose(file);

	NetpbmImage* image = new NetpbmImage(netpbmFormat, width, height, byteSize, bytesBuffer);

	return image;
}

void NetpbmImage::autoBrightness()
{
}

void NetpbmImage::write(char* filename)
{
	FILE* file = fopen(filename, "wb");
	if (file == NULL)
		throw exception("Cannot create output file");

	int valuesCount = fprintf(file, HEADER_FORMAT, this->format, this->width, this->height, this->byteSize);
	
	fwrite(this->bytes, sizeof(byte), this->bytesCount, file);

	fclose(file);
}

NetpbmImage::~NetpbmImage()
{
	delete[] this->bytes;
}
