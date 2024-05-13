#include <iostream>

class Task
{
public:
    void execute()
    {
        std::cout << "Task is excuting" << std::endl;
    }
};
/*
定义任务类：首先需要定义一个任务类，用于封装需要在线程池中执行的任务。
任务类至少应该包含一个执行任务的方法，可以是一个函数指针或者是一个函数对象
*/