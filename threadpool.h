#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "task.h"
/*
定义线程池类：接下来定义线程池类，其中包含了线程池的管理逻辑，
如线程的创建、销毁、任务的添加等。线程池类需要包含一个线程池容器，用于存放线程对象。
*/
class ThreadPool
{
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();

    void addTask(Task* task);
private:
    std::vector<std::thread> workers;  // 线程池中的线程
    std::queue<Task*> tasks;           // 任务队列
    std::mutex queueMutex;             // 保护任务队列的互斥量
    std::condition_variable condition; // 用于线程间通信的条件变量
    bool stop;                         // 标志线程池是否停止的标志位
};