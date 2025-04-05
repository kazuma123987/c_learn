#include "my_vector.h"
#include <stdio.h>
int main(int argc, char *argv[])
{  
    Vector *v = vector_create(1);
    for (int i = 0; i < 100; i++)
        vector_pushback(v, i);
    vector_free(&v);
#if defined(__GNUC__)
    printf("the compiler is gcc\n");
#elif defined(__clang__)
    printf("the compiler is clangc\n");
#endif

#ifdef USE_GCC_A
    printf("USE_GCC_A is defined");
#endif
    return 0;
}