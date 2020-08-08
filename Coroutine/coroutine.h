#ifndef __COROUTINE_H__
#define __COROUTINE_H__

#include <ucontext.h>

#define CORSZ 1024
#define STACKSZ (1024*64)

enum State {
	DEAD, // 死亡状态
	READY, // 就绪状态
	RUNNING, // 运行状态
	SUSPEND // 挂起状态
};

struct schedule;

// 协程结构体
typedef struct {
	void* (*call_back)(struct schedule* s, void* args); // 回调函数
	void* args; // 回调函数参数
	ucontext_t ctx; // 协程上下文
	char stack[STACKSZ]; // 协程栈
	enum State state; // 协程状态
}coroutine_t;

// 协程调度器
typedef struct schedule {
	coroutine_t** coroutines; // 所有协程
	int current_id; // 当前正在运行的协程,如果没有正在运行的协程,置为-1
	int max_id; // 最大的下标
	ucontext_t ctx_main; // 主流程上下文
}schedule_t;

schedule_t* schedule_create(); // 创建调度器

int coroutine_create(schedule_t* s, void* (*call_back)(schedule_t*, void* args), void* args); // 创建协程,返回当前协程在调度器的下标

void coroutine_running(schedule_t* s, int id); // 启动协程 

void coroutine_yield(schedule_t* s); // 让出CPU

void coroutine_resume(schedule_t* s, int id); // 恢复CPU

void schedule_destroy(schedule_t* s); // 释放调度器

int schedule_finished(schedule_t* s); // 判断是否所有协程都运行完了

#endif // __COROUTINE_H__
