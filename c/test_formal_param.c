#include <stdio.h>

void print_arr_addr(int test_addr[])
{
    printf("printf arr test_addr = %p\n", test_addr);
    test_addr++;
    printf("printf arr test_addr = %p\n", test_addr);
}

void print_ptr_addr(int *p_test_addr)
{
    printf("printf ptr p_test_addr = %p\n", p_test_addr);
    p_test_addr++;
    printf("printf ptr p_test_addr = %p\n", p_test_addr);
}

int main(void)
{
    int test_int_arr[10];

    print_arr_addr(test_int_arr);

    print_ptr_addr(test_int_arr);

    return 0;
}
