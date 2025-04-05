#include "func_pointer.h"
int main(int argc, char *argv[])
{
    printf("1+2=%d\n1-2=%d\n1*2=%d\n", myadd(1, 2), mydec(1, 2), mymul(1, 2));
    func1 f1 = add;
    func2 *f2 = &add;
    func3 = add;
    return 0;
}