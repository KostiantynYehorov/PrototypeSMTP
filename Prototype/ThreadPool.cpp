#include "ThreadPool.h"

ThreadPool::ThreadPool(int thread_size)
{
    this->thread_size = thread_size;
    is_stop = false;

}

ThreadPool::~ThreadPool()
{
    Join();

    if (!IsStopped())
    {
        Stop();
    }
}

void ThreadPool::Join()
{
    for (int i = 0; i < thread_size; ++i)
    {
        thread_pool.at(i).join();
    }
}

void ThreadPool::Stop()
{
    is_stop = true;

    for (int i = 0; i < thread_size; ++i)
    {
        thread_pool.pop_back();
    }
}

void ThreadPool::DoTask()
{
    while (!is_stop && !task.empty())
    {
        std::unique_lock<std::mutex> lck(m);

        while (task.empty())
        {
            task_not_empty.wait(lck);
        }

        auto t_task = task.begin()->first;
        auto s_socket = task.begin()->second;

        task.erase(task.begin());
        lck.unlock();

        t_task(s_socket);
    }
}

void ThreadPool::AddTask(void* in_task, SOCKET socket)
{
    std::unique_lock<std::mutex> lck(m);
    task.emplace((RUNFUC)in_task, socket);

    if (task.size() == 1)
    {
        task_not_empty.notify_one();
    }

    thread_pool.emplace_back(&ThreadPool::DoTask, this);
}

void ThreadPool::set_size(int thread_size)
{
    this->thread_size = thread_size;
}