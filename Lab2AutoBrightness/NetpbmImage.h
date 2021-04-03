#pragma once

class NetpbmImage
{
private:
	char format[3];
	int width, height;
	unsigned char* bytes;

public:
	void Read(char* filename);
	void AutoBrightness();
	void Write(char* filename);
};