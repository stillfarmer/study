#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int main()
{
    struct timeval tv;
	gettimeofday(&tv, NULL);

	printf("tv.sec %u\n", tv.tv_sec);
	printf("tv.usec %u\n", tv.tv_usec);
}
