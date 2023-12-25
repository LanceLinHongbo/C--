#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int singleNumber(const int *nums, int numsSize)
{
    int result = 0;

    for (int i = 0; i != numsSize; ++i)
    {
        result ^= nums[i];
    }
    return result;
}

int main()
{
    int arr[] = {4, 1, 2, 1, 2};
    int result = singleNumber(arr, 5);
    return 0;
}
