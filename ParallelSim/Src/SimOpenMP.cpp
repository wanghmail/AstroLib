#include <iostream>
#include <stdio.h>
#include <time.h>
#include "AstroLib.h"
#include "omp.h"



using namespace std;



void GetMPResult()
{
    int a = 0;
    double sum=0;
    for (int i = 0; i<10000000; i++)
    {
        for (int j=0; j<10; j++)
            sum += sin(i);
    }
}


void SimOpenMP()
{
    //获取开始执行时间
    AsMTimerStart(OpemMP_Parallel);

    //调用4个子线程同时进行计算
    omp_set_num_threads(4);
#pragma omp parallel for
    for (int i = 0; i < 4; i++)
    {
        GetMPResult();
    }

    AsMTimerStop(OpemMP_Parallel);


    //获取开始执行时间
    AsMTimerStart(Serial);

    for (int i = 0; i < 4; i++)
    {
        GetMPResult();
    }
    AsMTimerStop(Serial);
}
