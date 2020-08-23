ucontext 组件简介: 

在类 System V 环境下，头文件< ucontext.h > 中定义了两个结构类型 mcontext_t 和 ucontext_t 和四个函数 getcontext( )，setcontext( )，makecontext( )和 swapcontext( )。
利用它们可以在一个进程中实现用户级的 线程切换。mcontext_t 类型与机器相关，并且不透明。ucontext_t 结构体则至少拥有以下几个域: 

typedef struct ucontext { 
    struct ucontext *uc_link; 
    sigset_t         uc_sigmask; 
    stack_t          uc_stack; 
    mcontext_t       uc_mcontext; 
    ... 
} ucontext_t;

其中 sigset_t 和 stack_t 定义在标准头文件 <signal.h> 中，uc_link 字段保存当前context结束后继续执行的context记录，uc_sigmask 记录该context运行阶段需要屏蔽的信号，uc_stack
是该context运行的栈信息，最后一个字段uc_mcontext 则保存具体的程序执行上下文——如PC值、堆栈指针、寄存器值等信息——其实现方式依赖于底层运行的系统架构，是平台、硬件相关的。
