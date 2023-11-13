// Atomic.cpp : 定义控制台应用程序的入口点。
//

#include "MultiThread.h"
#include <iostream>
#include <cstdint>
#include <vector>
#include <thread>
#include <atomic>// <- atomic datatypes
#include <mutex>
#include "AstroLib.h"


using namespace std;


//多线程同步
void MultiThreadAtomic()
{
    std::mutex mutex;
    std::vector<std::thread> threads;
    const uint64_t num_threads =10;
    const uint64_t num_iters=100000000;

    auto lock_count = [&](volatile uint64_t* counter,
        const auto& id)-> void
    {
        for (uint64_t i=id; i<num_iters; i+=num_threads)
        {
            std::lock_guard<std::mutex> lock_guard(mutex);
            (*counter)++;
        }
    };

    auto atomic_count=[&](volatile std::atomic<uint64_t>* counter,
        const auto& id)-> void
    {
        for (uint64_t i=id; i<num_iters; i+=num_threads)
            (*counter)++;
    };

    AsMTimerStart(mutex_multithreaded)
    uint64_t counter =0;
    threads.clear();
    for (uint64_t id=0; id<num_threads; id++)
        threads.emplace_back(lock_count, &counter, id);
    for (auto& thread : threads)
        thread.join();
    AsMTimerStop(mutex_multithreaded)

    AsMTimerStart(atomic_multithreaded)
    std::atomic<uint64_t> atomic_counter(0);
    threads.clear();
    for (uint64_t id=0; id<num_threads; id++)
        threads.emplace_back(atomic_count, &atomic_counter, id);
    for (auto& thread : threads)
        thread.join();
    AsMTimerStop(atomic_multithreaded)

    std::cout << counter << " " << atomic_counter << std::endl;
}






