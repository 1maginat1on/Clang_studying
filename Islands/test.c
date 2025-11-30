#include <stdio.h>

#define max_size 10

int A[max_size][max_size];

void func(int B[max_size][max_size])
{
    B[0][0] = 1;
}


int main(void)
{
    func(A);

    printf("%d", A[0][0]);


    return 0;
}