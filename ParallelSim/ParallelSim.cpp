// ParallelCpp.cpp : 定义控制台应用程序的入口点。
//

#include <fstream>
#include <iostream>
#include <cstdint>
#include "MultiThread.h"
#include "SimOpenMP.h"


using namespace std;



int main()
{
    printf("*****************************************************************\n");
    printf("*                                                               *\n");
    printf("*        <<Aerospace System Modeling and Simulation>>           *\n");
    printf("*                      Parellel Sim                             *\n");
    printf("*                                                               *\n");
    printf("*****************************************************************\n\n");

    //C++多线程仿真
    //MultiThreadStatic();
    //MultiThreadAtomic();
    //MultiThreadDynamic();

    //OpenMP仿真
    SimOpenMP();

    int i;
    cin>>i;

    return 0;
}


