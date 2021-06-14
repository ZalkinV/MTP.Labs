#pragma once

float* readArray(const char* fileName, size_t* length);

void printArray(const float* arr, const size_t length);
void fprintArray(const char* fileName, const float* arr, const size_t length);
void printArrayToStream(FILE* stream, const float* arr, const size_t length);
