#include <stdio.h>
#include <stdlib.h>

int fib(int n)
{
    if (n < 3) return 1;
    else return fib(n - 2) + fib(n - 1);
}

int main(int argc, char* argv[])
{
    int n = atoi(argv[1]);
    int res = fib(n);
    printf("%d fibonacci number is: %d \n", n, res);

    return 0;
}
