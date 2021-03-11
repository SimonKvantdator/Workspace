#include <stdio.h>
#include <math.h>

#define SIZE (int)pow(10, 7)

int main()
{
    int as[SIZE];
    for (size_t ix = 0; ix < SIZE; ++ix)
        as[ix] = 0;

    printf("%d\n", as[0]);

    return 0;
}

