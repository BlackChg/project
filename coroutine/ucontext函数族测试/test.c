#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ucontext.h>

int main()
{
	int i = 0;
	ucontext_t ctx;

	getcontext(&ctx);//获取当前上下文
	printf("i = %d\n", i++);
	sleep(1);

	setcontext(&ctx);//回复ucp上下文
	return 0;
}
