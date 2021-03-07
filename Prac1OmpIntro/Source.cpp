#include <iostream>
#include <omp.h>

void Task1()
{
    printf("%i", omp_get_num_threads());
#pragma omp parallel
    {
        printf("%i / %i\n", omp_get_thread_num(), omp_get_num_threads());
        printf("Hello World!\n");
    }
}

int CalcSum(int* numbers, int length)
{
    int sum = 0;
    for (int i = 0; i < length; i++)
    {
        sum += numbers[i];
    }

    return sum;
}

int MultiThreadCalcSum1(int* numbers, int length)
{
    int threadsCount = omp_get_max_threads();
    int* multiSums = new int[threadsCount];
#pragma omp parallel
    {
        int threadIndex = omp_get_thread_num();
        multiSums[threadIndex] = 0;

        int iMax = ceil(length / threadsCount);
        for (int i = 0; i < iMax; i++)
        {
            int indexToSum = threadIndex + (i * threadsCount);
            if (indexToSum < length)
                multiSums[threadIndex] += numbers[indexToSum];
        }
    }
    int multiSum = CalcSum(multiSums, threadsCount);

    return multiSum;
}

int MultiThreadCalcSum2(int* numbers, int length)
{
    int threadsCount = omp_get_max_threads();
    int* multiSums = new int[threadsCount];
    
    int numbersPerThreadCount = length / threadsCount;
    int remainingNumbers = length % threadsCount;

#pragma omp parallel
    {
        int threadIndex = omp_get_thread_num();
        multiSums[threadIndex] = 0;

        int firstIndexShift = 0;
        int lastIndexShift = 0;
        if (remainingNumbers > 0)
        {
            bool hasThreadExtraNumber = threadIndex < remainingNumbers;
            firstIndexShift = hasThreadExtraNumber ? threadIndex : remainingNumbers;
            lastIndexShift = hasThreadExtraNumber ? 1 : 0;
        }

        int firstIndex = threadIndex * numbersPerThreadCount + firstIndexShift;
        int lastIndex = firstIndex + numbersPerThreadCount + lastIndexShift;

        for (int i = firstIndex; i < lastIndex; i++)
        {
            multiSums[threadIndex] += numbers[i];
        }
    }

    int multiSum = CalcSum(multiSums, threadsCount);

    return multiSum;

    /*int firstIndex = threadIndex * numbersPerThreadCount;
    int lastIndex = firstIndex + numbersPerThreadCount;

    if (remainingNumbers > 0)
    {
        bool hasThreadExtraNumber = threadIndex < remainingNumbers;
        int firstIndexShift = hasThreadExtraNumber ? threadIndex : remainingNumbers;
        int lastIndexShift = firstIndexShift + (hasThreadExtraNumber ? 1 : 0);

        firstIndex += firstIndexShift;
        lastIndex += lastIndexShift;
    }*/
}

int MultiThreadCalcSumExpected(int* numbers, int length)
{
    int threadsCount = omp_get_max_threads();
    int* multiSums = new int[threadsCount];

#pragma omp parallel
    {
        int threadIndex = omp_get_thread_num();
        multiSums[threadIndex] = 0;

        int firstIndex = length * threadIndex / threadsCount;
        int lastIndex = length * (threadIndex + 1) / threadsCount;

        for (int i = firstIndex; i < lastIndex; i++)
        {
            multiSums[threadIndex] += numbers[i];
        }
    }

    for (int i = 0; i < threadsCount; i++)
    {
        int firstIndex = length * i / threadsCount;
        int lastIndex = length * (i + 1) / threadsCount;
        printf_s("Thread #%d = %d\t\t %d - %d\n", i, multiSums[i], firstIndex, lastIndex);
    }

    int multiSum = CalcSum(multiSums, threadsCount);

    return multiSum;
}

void Task2()
{
    srand(1);

    // 1
    int n;
    scanf_s("%i", &n);

    // 2
    int* numbers = new int[n];
    for (int i = 0; i < n; i++)
        numbers[i] = rand() % 100;

    // 3
    int singleSum = CalcSum(numbers, n);

    // 4
    int multiSum = MultiThreadCalcSumExpected(numbers, n);

    //4.4
    printf("single_sum=%i multi_sum=%i\n", singleSum, multiSum);
    printf("Sums are%s equal", singleSum != multiSum ? " not" : "");
}

int main()
{
    Task2();
}
