#include "threadpool.h"

ThreadPool::ThreadPool(size_t numThreads)
: stop(false)
{
    for(size_t i = 0; i < numThreads; ++i) 
    {
        workers.emplace_back([this] {
            while(true)
            {
                Task* task = nullptr;
                // 此线程取任务的时候  别的线程不可以取 所以必须上锁
                // 其他线程无法同时访问任务队列
                std::unique_lock<std::mutex> lock(queueMutex);
                // condition调用wait方法 使线程在条件变量condition上等待 
                // 当不满足条件时 会释放锁lock 满足条件时(stop为true 或 tasks不为空 满足之一时) 重新获取lock 并向下继续执行
                // 假如此时没有任务了 线程会阻塞在这里 直到任务队列有新任务到来时 该线程会被再次激活
                condition.wait(lock, [this] {return stop || !tasks.empty();});
                // 线程池停止 且 任务队列为空时 直接返回 默认情况下stop为false
                if(stop && tasks.empty())
                {
                    return;
                }
                // 获取并执行任务
                task = tasks.front();
                tasks.pop();
                task->execute();
                delete task;
            }
        });
    }
}

ThreadPool::~ThreadPool()
{
    std::unique_lock<std::mutex> lock(queueMutex);
    stop = true;
    condition.notify_all();
    for(std::thread& work : workers)
    {
        work.join();
    }
}

void ThreadPool::addTask(Task* task)
{
    std::unique_lock<std::mutex> lock(queueMutex);
    tasks.push(task);
    // 向等待condition条件变量的一个线程发出通知
    // 告诉它有新的任务可执行 可以取出任务
    condition.notify_one();
}