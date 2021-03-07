#include<iostream>
#include<omp.h>
#include<ctime>


void print_sums(int single_sum, int multi_sum)
{
    printf("single_sum=%i multi_sum=%i\n", single_sum, multi_sum);
    printf("Sums are%s equal\n", single_sum != multi_sum ? " NOT" : "");
}

int calc_single_sum(int* numbers, int length)
{
    int sum = 0;
    for (int i = 0; i < length; i++)
        sum += numbers[i];

    return sum;
}

int standard_multithread(int* numbers, int length)
{
    printf(__FUNCTION__);
    for (int i = 0; i < 1000; i++)
        printf("1");
    return 0;
}

int cool_multithread(int* numbers, int length)
{
    printf(__FUNCTION__);
    int a = 0;
    for (int i = 0; i < 10000; i++)
        a += i;
    return 0;
}

void test_method(int (*func)(int*, int), int* numbers, int length)
{
    printf("Execution of '");

    clock_t start = clock();
    int multi_sum = func(numbers, length);
    clock_t end = clock();
    int milliseconds = end - start;

    printf("' took %d milliseconds with following result:\n", milliseconds);

    int single_sum = calc_single_sum(numbers, length);
    print_sums(single_sum, multi_sum);
    printf("\n");
}

int main()
{
    srand(1);

    int n;
    scanf_s("%i", &n);

    int* numbers = new int[n];
    for (int i = 0; i < n; i++)
        numbers[i] = rand() % 100;

    test_method(standard_multithread, numbers, n);
    test_method(cool_multithread, numbers, n);
}
