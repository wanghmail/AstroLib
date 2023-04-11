#pragma once
///***********************************************************************
/// 方向余弦矩阵转为312转序Euler角
/// @Author	Zhang Xuaying
/// @Date	2023.04.01
/// @Input  Mtx  	   方向余弦矩阵
/// @Param	
/// @Param	
/// @Output	eulerangles	欧拉角
///***********************************************************************
#include<iostream>
#include "SimDoF6.h"
#include "DoF6.h"
using namespace std;
CVector<double> AsMtxToEuler(CMatrix<double>& Mtx);