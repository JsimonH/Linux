**协程**（Coroutine，又称微线程）是一种比线程更加轻量级的存在，协程不是被操作系统内核所管理，而完全是由程序所控制。协程可以比作子程序，但在执行过程中，子程序内部可中断，然后转而执行别的子程序，在适当的时候再返回来接着执行。协程之间的切换不需要涉及任何系统调用或任何阻塞调用，协程只在一个线程中执行，是子程序之间的切换，发生在用户态上。而且，线程的阻塞状态是由操作系统内核来完成，发生在内核态上，因此协程相比线程节省线程创建和切换的开销。协程适用于IO阻塞且需要大量并发的场景，下面实现一个协程版本的网络服务器。
*****
### 实现思路
主循环不断的调用accept，如果有连接，创建新的协程。如果没有连接，恢复协程。每个协程中，如果收到数据，就处理，如果没有数据，就放弃CPU。

协程的核心就是**用户态实现栈切换**。Linux提供了getcontext()，setcontext()，makecontext()，swapcontext()四个函数实现栈切换。我们首先来了解**ucontext组件：**
    在类 System V 环境下，头文件< ucontext.h > 中定义了两个结构类型 mcontext_t 和 ucontext_t 和四个函数 getcontext( )，setcontext( )，makecontext( )和 swapcontext( )。利用它们可以在一个进程中实现用户级的线程切换。mcontext_t 类型与机器相关，并且不透明。ucontext_t 结构体则至少拥有以下几个域: 

**typedef struct ucontext {
    struct ucontext *uc_link*; // 当前上下文执行完了，恢复运行的上下文
    stack_t uc_stack;          // 该上下文中使用的栈
    mcontext_t uc_mcontext;    // 保存所有的寄存器的值
    __sigset_t uc_sigmask;     // 该上下文中的阻塞信号集合
} ucontext_t;**

// 获取当前上下文保存到ucp中。
int getcontext(ucontext_t *ucp);  

// 设置上下文为ucp的执行状态。
int setcontext(const ucontext_t *ucp); 
 
 // 创建上下文，修改通过getcontext取得的上下文ucp, 然后给该上下文指定一个栈空间ucp->stack，设置后继的上下文ucp->uc_link。
void makecontext(ucontext_t *ucp, void (*func)(), int argc, ...);

// 切换上下文，保存当前上下文到oucp结构体中，然后激活upc上下文。
int swapcontext(ucontext_t *oucp, ucontext_t *ucp); 
