#include <iostream>
#include <pthread.h>
using namespace std;

// 对互斥锁的封装
class Mutex
{
public:
	Mutex()
	{
		pthread_mutex_init(&mutex, NULL); // 初始化锁
	}
	
	~Mutex()
	{
		pthread_mutex_destroy(&mutex); // 销毁锁
	} 
	
	void lock()
	{
		pthread_mutex_lock(&mutex); // 获取锁
	}
	
	void unlock()
	{
		pthread_mutex_unlock(&mutex); // 释放锁
	}

private:
	pthread_mutex_t mutex;
};

class Singleton
{
public:
	static Singleton* getInstance()
	{
		if (NULL == object)
		{
			// 获取互斥锁
			mutex.lock();
			/* 
			这里需要再添加一个if判断，否则当两个
			线程都进入这里，又会多次new对象，不符合
			单例模式的设计
			*/
			if(NULL == object)
			{
				object = new Singleton();
			}
			// 释放互斥锁
			mutex.unlock();
		}
		return object;
	}

private:
	Singleton() 
	{ 
		cout << "Singleton()" << endl; 
	}
	
	~Singleton() 
	{ 
		cout << "~Singleton()" << endl;
	}
	
	Singleton(const Singleton&);
	static Singleton *object;
	
	class Release
	{
	public:
		~Release() 
		{ 
			delete object; 
		}
	};
	static Release release;
	
	//线程间的静态互斥锁
	static Mutex mutex;
};
Singleton* Singleton::object = NULL;
Singleton::Release Singleton::release;
// 定义互斥锁静态对象
Mutex Singleton::mutex;

int main()
{
	Singleton *p1 = Singleton::getInstance();
	Singleton *p2 = Singleton::getInstance();
	Singleton *p3 = Singleton::getInstance();
	return 0;
}
