#include <stdio.h>  
#include <string.h>
#include <sys/resource.h>
#include <errno.h>
#include <sys/stat.h>

static int EnableCoreDump()
{
	struct rlimit core_limit;

	if(getrlimit(RLIMIT_CORE, &core_limit) < 0)
	{
		printf("get limit fail!\n");
		return 0;
	}
	printf("Before set up limit: software=%lu, hardware=%lu\n", core_limit.rlim_cur, core_limit.rlim_max);

	core_limit.rlim_cur = RLIM_INFINITY;
	core_limit.rlim_max = RLIM_INFINITY;
	if (setrlimit(RLIMIT_CORE, &core_limit) < 0)
	{
		fprintf(stderr, "setrlimit: %s\nWarning: core dumps may be truncated or non-existant\n", strerror(errno));
		return 0;
	}

	if(getrlimit(RLIMIT_CORE, &core_limit) < 0)
	{
		printf("get limit fail!\n");
		return 0;
	}
	printf("After set up limit: software=%lu, hardware=%lu\n", core_limit.rlim_cur, core_limit.rlim_max);

	return 1;
}

char names[] = "Oops! here is a buffer overflow, wcdj";  
  
void func()                                               
{  
    char buf[5];  
    strcpy(buf, names);  
}  
  
int main()  
{
	EnableCoreDump();
  
    func();  
    return 0;  
}  
