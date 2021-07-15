// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include<iostream>

#include <winsock2.h>
#include <ws2tcpip.h>

#include <condition_variable>
#include <mutex>
#include <vector>
#include <thread>
#include <map>
#include <chrono>
#include <memory>
#include <functional>

#pragma comment(lib, "Ws2_32.lib")

typedef void (*RUNFUC)(SOCKET);

class ThreadPool
{
public:
    ThreadPool() : is_stop(false), thread_size(0) {}
    ThreadPool(int thread_size);
    ~ThreadPool();

    void AddTask(void* in_task, SOCKET socket);

    void set_size(int thread_size);

private:
    void Join();
    void Stop();
    void DoTask();

    bool IsStopped() { return is_stop; };

private:
    int thread_size;
    bool is_stop;

    std::map<RUNFUC, SOCKET> task;

    std::mutex m;

    std::vector<std::thread> thread_pool;

    std::condition_variable task_not_empty;
};

