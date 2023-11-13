// Atomic.cpp : 定义控制台应用程序的入口点。
//

#include "MultiThread.h"
#include <iostream>
#include <cstdint>
#include <vector>
#include <thread>
#include <numeric>
#include "AstroLib.h"

using namespace std;



//计算函数
void GetResult(int id, int num_threads, const std::vector<int>& largeArrays, double& result)
{
    int num = largeArrays.size();
    int lower = id*num/num_threads;
    int upper = (id+1)*num/num_threads;
    if (id==num_threads-1)
        upper = num;

    for (int i=lower; i<upper; i++)
    {
        for (int j=0; j<10; j++)
            result += sin(largeArrays[i]);
    }
}


//静态分发
void StaticScheduling(
    std::vector<int>& largeArrays,
    int num_threads=2)
{
    //创建子线程
    std::vector<std::thread> threads;
    vector<double> results(num_threads);

    for (int id=0; id<num_threads; id++)
    {
        results[id] = 0;
        threads.emplace_back(GetResult, id, num_threads, std::cref(largeArrays), std::ref(results[id]));
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


//多线程静态分发
void MultiThreadStatic()
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
    StaticScheduling(largeArrays,4);
    AsMTimerStop(compute_result)
}


