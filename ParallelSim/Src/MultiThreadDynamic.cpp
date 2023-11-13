#include <iostream>                   // std::cout
#include <cstdint>                    // uint64_t
#include <vector>                     // std::vector
#include <thread>                     // std::thread (not used yet)
#include <algorithm> 
#include "AstroLib.h"

#include <mutex>
#include <atomic>



using namespace std;


//动态分发
void DynamicScheduling(
    std::vector<int>& largeArrays,
    int num_threads=4,
    int chunk_size=64)
{

    // declare mutex and current lower index
    int global_lower = 0;
    int num = largeArrays.size();

    auto dynamic_block_cyclic = [&] (int id, double& result ) -> void
    {
        // assume we have not done anything
        int lower = 0;

        // while there are still rows to compute
        while (lower < num) {

            // update lower row with global lower row
            {
                std::lock_guard<std::mutex> guard(mutex);
                       lower  = global_lower;
                global_lower += chunk_size;
            }

            // compute the upper border of the block (exclusive)
            const int upper = std::min(lower+chunk_size, num);

            for (int i=lower; i<upper; i++)
            {
                for (int j=0; j<10; j++)
                    result += sin(largeArrays[i]);
            }
        }
    };

    //创建子线程
    std::vector<std::thread> threads;
    vector<double> results(num_threads);

    for (int id=0; id<num_threads; id++)
    {
        results[id] = 0;
        threads.emplace_back(dynamic_block_cyclic, id, std::ref(results[id]));
    }

    //主线程要等待子线程执行完毕
    for (auto& thread : threads)
        thread.join();

    //汇总各个子线程的结果
    double resultSum = 0;
    for (int id=0; id<num_threads; id++)
    {
        resultSum += results[id];
    }

}


//多线程动态分发
void MultiThreadDynamic()
{
    AsMTimerStart(ini_data)
    long long num = 4000000;
    vector<int> largeArrays(num);
    for (int i=0; i<num; i++)
    {
        largeArrays[i] = 1;
    }
    AsMTimerStop(ini_data)

    AsMTimerStart(compute_result)
    DynamicScheduling(largeArrays);
    AsMTimerStop(compute_result)

}

