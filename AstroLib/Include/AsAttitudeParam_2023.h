#pragma once

#include "AsCoordinate.h"
#include "AsMath.h"
#include "AsAttitudeParam.h"




///***********************************************************************
/// The conversion from matrix to axis and angle.
/// @Author	Xiao Yao
/// @Date	2023.4.2
/// @Input
/// @Param	mtx	    坐标转换矩阵
/// @Output
/// @Param	axis	旋转轴矢量
/// @Param	angle	旋转角[0, pi]
///***********************************************************************
void	AsMtxToAxAng(const CMatrix<double>& mtx, CCoord& axis, double& angle);

///***********************************************************************
///将四元数转化成欧拉角_123转序
/// @Author	Liu Tianqing
/// @Date	2023.04.06
/// @Input
/// @Param	q		标准化四元数
/// @Output
/// @Param	euler	欧拉角
///***********************************************************************
bool	AsQuatToEuler123(const CQuaternion& q, CEuler& euler);

///***********************************************************************
/// 方向余弦阵转321转序欧拉角
/// @Author	Li Sibei
/// @Date	2023.04.06
/// @Input
/// @Param	mtx		方向余弦阵
/// @Output
/// @Param	euler	欧拉角
///***********************************************************************
void	AsMtxToEuler321(const CMatrix<double>& mtx, CEuler& euler);

///***********************************************************************
/// 方向余弦矩阵转为313转序Euler角
/// @Author	Wang Weili
/// @Date	2023.4
/// @Input
/// @Param	mtx 坐标转移矩阵
/// @Output
/// @Param	euler 欧拉角
///***********************************************************************
void	AsMtxToEuler313(const CMatrix<double>& mtx, CEuler& euler);

///***********************************************************************
/// 方向余弦矩阵转为312转序Euler角
/// @Author	Zhang Xuaying
/// @Date	2023.04.01
/// @Input  
/// @Param	mtx  		方向余弦矩阵
/// @Output	
/// @Param	euler		欧拉角(rad)
///***********************************************************************
void	AsMtxToEuler312(const CMatrix<double>& mtx, CEuler& euler);

///******************************************************
/// 根据转序和坐标转移矩阵计算Euler角，转序为2-3-1
/// @author Sun Fengyan
/// @data   2023.4
/// @Version    1.0
/// @Input  
/// @Param  mtx       坐标转移矩阵
/// @Output
/// @Param  euler     欧拉角返回值(单位：rad）
///******************************************************
void	AsMtxToEuler231(const CMatrix<double>& mtx, CEuler& euler); 

///***********************************************************************
/// 方向余弦阵转123转序欧拉角
/// @Author	Gong he
/// @Date	2023.04.14
/// @Input
/// @Param	mtx		方向余弦阵
/// @Output
/// @Param	angle	欧拉角
///***********************************************************************
void	AsMtxToEuler123(const CMatrix<double>& mtx, CEuler& euler);