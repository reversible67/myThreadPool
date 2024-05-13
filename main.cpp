#include "threadpool.h"

int main()
{
    ThreadPool pool(4);   // 创建一个包含4个线程的线程池

    // 添加任务到线程池
    for(int i = 0; i < 8; ++i)
    {
        pool.addTask(new Task());
    }

    return 0;
}