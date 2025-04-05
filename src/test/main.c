#include <stdio.h>

int func(int a,short b)
{
    int c = 0x12345678;
    return a+b;
}

int main(int argc, char *argv[])
{
    func(3,4);
    return 0;
}