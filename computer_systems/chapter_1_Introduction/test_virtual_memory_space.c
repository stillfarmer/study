#include <stdio.h>

int g_test = 0;

int main( void)
{
    int i = 0;

    printf("g_test addr  = %p\n", &g_test);
    printf("i addr       = %p\n", &i);
    printf("printf addr  = %p\n", printf);
    printf("scanf addr   = %p\n", scanf);
    printf("fprintf addr = %p\n", fprintf);

    return 0;
}
