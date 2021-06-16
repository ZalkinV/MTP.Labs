#pragma once

void runTest(float* arr, size_t arrLength, size_t localGroupSize, size_t chunksCount);

void startTests();
void testLabExample();
void testSwanExample();

float* generateRandomArray(size_t length);
float* generateSequentialArray(size_t length);

bool equals(float* firstArr, float* secondArr, size_t length);
void printWrongArraysInfo(float* expected, float* actual, size_t length);
