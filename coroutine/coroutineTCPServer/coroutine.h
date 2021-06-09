#ifndef __COROUTINE_H__
#define __COROUTINE_H__

#include <ucontext.h>

#define COR_SIZE (1024)
#define STACK_SIZE (1024*64)

struct schedule;

enum State {DEAD, READY, RUNNING, SUSPEND};

typedef struct coroutine
{
	void *(*call_back)(struct schedule *s, void* args);//回调函数
	void *args; //回调函数参数
	ucontext_t ctx; //协程上下文
	char stack[STACK_SIZE]; //协程栈
	enum State state; //协程状态
}coroutine_t;

typedef struct schedule
{
	coroutine_t** coroutines;//协程数组
	int cur_id;//当前运行的协程
	int max_id;//数组的最大协程数
	ucontext_t ctx_main;//主流程上下文
}schedule_t;

//创建协程调度器
schedule_t* schedule_create();

//创建协程，返回当前协程在调度器的下标
int coroutine_create(schedule_t* s, void*(*call_back)(schedule_t* s, void *args), void *args);

//启动协程
int coroutine_running(schedule_t* s, int id);

//协程让出CPU，返回主流程上下文
void coroutine_yield(schedule_t* s);

//恢复协程上下文
void coroutine_resume(schedule_t* s, int id);

//销毁调度器
void schedule_destroy(schedule_t* s);

//判断调度器中的协程是否都全部运行结束，全结束返回1，否则返回0
int schedule_finished(schedule_t *s);

#endif //__COROUTINE_H__
