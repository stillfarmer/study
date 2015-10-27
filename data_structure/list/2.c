#include <stdio.h>

#define container_of(ptr, type, member) ({      \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
	    (type *)( (char *)__mptr - offsetof(type,member) );})  

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

struct test_struct {  
	int num;  
	char ch;  
	float fl;  
};  

int main(void)  
{  
	struct test_struct init_test_struct = { 99, 'C', 59.12 };  

	char *char_ptr = &init_test_struct.ch;  

	struct test_struct *test_struct = container_of(char_ptr, struct test_struct, ch);  

	printf(" test_struct->num = %d\n test_struct->ch = %c\n test_struct->fl = %f\n",   
			test_struct->num, test_struct->ch, test_struct->fl);  

	return 0;  
}  
