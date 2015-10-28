#include <stdio.h>

/* This is important, without this offsetof macro, compile error */
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#ifndef container_of
/**
 * container_of - cast a member of a structure out to the containing structure
 *
 * @ptr:        the pointer to the member.
 * @type:       the type of the container struct this is embedded in.
 * @member:     the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({                      \
		const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
		(type *)( (char *)__mptr - offsetof(type,member) );})
#endif


struct test_struct
{
	int a;
	int member;
};

void main(void)
{
    struct test_struct wudi;
	int *p_member_int = &(wudi.member);
	wudi.member = 10;
	
	printf("wudi addr = %p\n", &wudi);
	printf("wudi.member value  = %d\n", wudi.member);
	printf("member addr = %p\n", &(wudi.member));
	printf("p_member_int value = %p\n", p_member_int);

    struct test_struct *p_test = container_of(p_member_int, struct test_struct, member);
    printf("containter of addr = %p\n", p_test);
}
