#include <exception>
#include <string>

#include "NetpbmImage.h"
#include "BytesQueue.h"

using namespace std;

// ?? ????????? ?++ \n ??????, ??? ?? ????????? ???? ??? ????????? ?????????? ????????, ???????, ????? ?? ?????????? ????????? ?????????? ??????? ? ?????? ????? ??????, ???????????? ????? ?????? ????????? ? %1\n, ????? ?????????? ?????? ???? ?????? ???????? ??????
const char* NetpbmImage::HEADER_FORMAT = "P%i\n%i %i\n%i%c";

NetpbmImage::NetpbmImage(NetpbmFormat format, int width, int height, int byteSize, byte* bytes)
{
	if (format != NetpbmFormat::P5 && format != NetpbmFormat::P6)
		throw exception("Wrong Netpbm file format");

	this->format = format;
	this->width = width;
	this->height = height;
	this->byteSize = byteSize;
	this->initialBytes = bytes;
	this->bytesCount = NetpbmImage::calculateBytesCount(format, width, height);
	
	this->resultBytes = new byte[this->bytesCount];
	memcpy(this->resultBytes, bytes, this->bytesCount);
}

int NetpbmImage::calculateBytesCount(NetpbmFormat format, int width, int height)
{
	int bytesCount = width * height;
	if (format == NetpbmFormat::P6)
		bytesCount *= 3;

	return bytesCount;
}

NetpbmImage* NetpbmImage::read(const char* filename)
{
	FILE* file = fopen(filename, "rb");
	if (file == NULL)
		throw exception("Cannot open input file");

	int format = 0, width = 0, height = 0, byteSize = 0; char newLineChar;
	int valuesCount = fscanf(file, HEADER_FORMAT, &format, &width, &height, &byteSize, &newLineChar);
	if (valuesCount != 5)
		throw exception("Wrong header in the input file");

	NetpbmFormat netpbmFormat = (NetpbmFormat)format;
	
	int bytesCount = NetpbmImage::calculateBytesCount(netpbmFormat, width, height);
	byte* bytesBuffer = new byte[bytesCount];
	fread(bytesBuffer, sizeof(byte), bytesCount, file);
	
	fclose(file);

	NetpbmImage* image = new NetpbmImage(netpbmFormat, width, height, byteSize, bytesBuffer);

	return image;
}

void NetpbmImage::autoBrightnessScheduleTest(int chunkSize)
{
	int thresholdPixelsCount = (this->width * this->height) / 256;
	BytesQueue bytesQueue(thresholdPixelsCount);

#pragma omp parallel for schedule(static, chunkSize)
	for (int i = 0; i < this->bytesCount; i++)
	{
		bytesQueue.push(this->initialBytes[i]);
	}

	byte minThreshold = bytesQueue.topMin();
	byte maxThreshold = bytesQueue.topMax();

#pragma omp parallel for schedule(static, chunkSize)
	for (int i = 0; i < this->bytesCount; i++)
	{
		byte curByte = this->initialBytes[i];
		byte newByte = scaleColor(curByte, minThreshold, maxThreshold);
		this->resultBytes[i] = newByte;
	}
}

void NetpbmImage::autoBrightness()
{
	int thresholdPixelsCount = (this->width * this->height) / 256;
	BytesQueue bytesQueue(thresholdPixelsCount);

#pragma omp parallel for schedule(guided)
	for (int i = 0; i < this->bytesCount; i++)
	{
		bytesQueue.push(this->initialBytes[i]);
	}

	byte minThreshold = bytesQueue.topMin();
	byte maxThreshold = bytesQueue.topMax();

#pragma omp parallel for schedule(dynamic, 32)
	for (int i = 0; i < this->bytesCount; i++)
	{
		byte curByte = this->initialBytes[i];
		byte newByte = scaleColor(curByte, minThreshold, maxThreshold);
		this->resultBytes[i] = newByte;
	}
}

void NetpbmImage::autoBrightnessST()
{
	int thresholdPixelsCount = (this->width * this->height) / 256;
	BytesQueue bytesQueue(thresholdPixelsCount);

	for (int i = 0; i < this->bytesCount; i++)
	{
		bytesQueue.push(this->initialBytes[i]);
	}

	byte minThreshold = bytesQueue.topMin();
	byte maxThreshold = bytesQueue.topMax();

	for (int i = 0; i < this->bytesCount; i++)
	{
		byte curByte = this->initialBytes[i];
		byte newByte = scaleColor(curByte, minThreshold, maxThreshold);
		this->resultBytes[i] = newByte;
	}
}

byte NetpbmImage::scaleColor(byte value, byte min, byte max)
{
	int newValue = (value - min) * 255 / (max - min);
	
	if (newValue < 0)
		newValue = 0;
	if (newValue > 255)
		newValue = 255;
	
	return (byte)newValue;
}

void NetpbmImage::write(char* filename)
{
	FILE* file = fopen(filename, "wb");
	if (file == NULL)
		throw exception("Cannot create output file");

	int valuesCount = fprintf(file, HEADER_FORMAT, this->format, this->width, this->height, this->byteSize, '\n');
	
	fwrite(this->resultBytes, sizeof(byte), this->bytesCount, file);

	fclose(file);
}

NetpbmImage::~NetpbmImage()
{
	delete[] this->initialBytes;
	delete[] this->resultBytes;
}
