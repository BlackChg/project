#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ucontext.h>

ucontext_t ctx_main, ctx_f1, ctx_f2;

void fun1()
{
	printf("fun1() start\n");
	swapcontext(&ctx_f1, &ctx_f2);
	printf("fun1() end\n");
}

void fun2()
{
	printf("fun2() start\n");
	swapcontext(&ctx_f2, &ctx_f1);
	printf("fun2 end\n");
}

int main()
{
	char stack1[8192];
	char stack2[8192];

	getcontext(&ctx_f1);
	getcontext(&ctx_f2);

	ctx_f1.uc_stack.ss_sp = stack1;
	ctx_f1.uc_stack.ss_size = 8192;
	ctx_f1.uc_stack.ss_flags = 0;
	ctx_f1.uc_link = &ctx_f2;
	makecontext(&ctx_f1, fun1, 0);

	ctx_f2.uc_stack.ss_sp = stack2;
	ctx_f2.uc_stack.ss_size = 8192;
	ctx_f2.uc_stack.ss_flags = 0;
	ctx_f2.uc_link = &ctx_main;
	makecontext(&ctx_f2, fun2, 0);

	swapcontext(&ctx_main, &ctx_f1);
	printf("main exit\n");
	return 0;
}
