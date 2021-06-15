#pragma once

void startTests();
void runTest(float* arr, size_t arrLength, size_t localGroupSize, size_t chunksCount);

void testLabExample();

bool equals(float* firstArr, float* secondArr, size_t length);
void printWrongArraysInfo(float* expected, float* actual, size_t length);
