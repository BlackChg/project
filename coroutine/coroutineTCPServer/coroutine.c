#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coroutine.h"

//创建协程调度器
schedule_t* schedule_create()
{
	schedule_t* s = (schedule_t*)malloc(sizeof(schedule_t));
	if (s != NULL)
	{
		s->coroutines = (coroutine_t**)malloc(sizeof(coroutine_t*)*COR_SIZE);
		memset(s->coroutines, 0x00, sizeof(coroutine_t*)*COR_SIZE);
		s->cur_id = -1;
		s->max_id = 0;
	}

	return s;
}

//协程执行函数
static void* start_fun(schedule_t* s)
{
	int id = s->cur_id;
	if (id != -1)
	{
		coroutine_t *c = s->coroutines[id];
		c->call_back(s, c->args);
		c->state = DEAD;//目标函数执行完毕，进行DEAD状态
		s->cur_id = -1;
	}
}

//创建协程，返回当前协程在调度器的下标
int coroutine_create(schedule_t* s, void*(*call_back)(schedule_t* s, void *args), void *args)
{
	coroutine_t *c = NULL;
	int i = 0;
	//找到空位置存放协程
	for ( ; i < s->max_id; ++i)
	{
		c = s->coroutines[i];
		if (c->state == DEAD)
			break;
	}
	//数组有效元素的下一个位置存放新的协程，或者这个协程是第一个进来的
	//则需要开辟空间存储
	if (i == s->max_id || c == NULL)
	{
		s->coroutines[i] = (coroutine_t*)malloc(sizeof(coroutine_t));
		s->max_id++;
	}
	
	//对协程进行初始化
	c = s->coroutines[i];
	c->call_back = call_back;
	c->args = args;
	c->state = READY;

	getcontext(&c->ctx);
	//对协程的上下文进行设置
	c->ctx.uc_stack.ss_sp = c->stack;
	c->ctx.uc_stack.ss_size = STACK_SIZE;
	c->ctx.uc_stack.ss_flags = 0;
	c->ctx.uc_link = &s->ctx_main;
	makecontext(&c->ctx, (void (*)())&start_fun, 1, s);

	return i;
}

//获取协程状态
static enum State coroutine_state(schedule_t* s, int id)
{
	coroutine_t* c = s->coroutines[id];

	if (c == NULL)
		return DEAD;
	return c->state;
}

//启动协程
int coroutine_running(schedule_t* s, int id)
{
	int st = coroutine_state(s, id);
	if (st == DEAD)
		return;
	coroutine_t* c = s->coroutines[id];
	c->state = RUNNING;
	s->cur_id = id;
	swapcontext(&s->ctx_main, &c->ctx);
}

//协程让出CPU，返回主流程上下文
void coroutine_yield(schedule_t* s)
{
	if (s->cur_id != -1)
	{
		coroutine_t* c = s->coroutines[s->cur_id];
		c->state = SUSPEND;
		s->cur_id = -1;
		swapcontext(&c->ctx, &s->ctx_main);
	}
}

//恢复协程上下文
void coroutine_resume(schedule_t* s, int id)
{
	coroutine_t *c = s->coroutines[id];
	if (c != NULL && c->state == SUSPEND)
	{
		c->state = RUNNING;
		s->cur_id = id;
		swapcontext(&s->ctx_main, &c->ctx);
	}
}

//删除协程
static void coroutine_delete(schedule_t* s, int id)
{
	coroutine_t* c = s->coroutines[id];
	if (c != NULL)
	{
		free(c);
		s->coroutines[id] = NULL;
	}
}

//销毁调度器
void schedule_destroy(schedule_t* s)
{
	int i;
	for (i = 0; i < s->max_id; ++i)
	{
		coroutine_delete(s, i);
	}

	free(s->coroutines);
	free(s);
}

//判断调度器中的协程是否都全部运行结束，全结束返回1，否则返回0
int schedule_finished(schedule_t *s)
{
	if (s->cur_id != -1)
		return 0;

	int i;
	for (i = 0; i < s->max_id; ++i)
	{
		coroutine_t* c = s->coroutines[i];
		if (c->state != DEAD)
			return 0;
	}
		
	return 1;
}
