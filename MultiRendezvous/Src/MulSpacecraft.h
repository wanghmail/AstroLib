// Spacecraft.h: interface for the CMulSpacecraft class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_MulSpacecraft_H__INCLUDED_)
#define _MulSpacecraft_H__INCLUDED_

#pragma once


#include "AstroLib.h"
#include <fstream>


//
//航天器动力学类,定义了航天器的参数和操作
//
class CMulSpacecraft
{
    //
    // Operation.
    //
public:
    CMulSpacecraft();
    virtual ~CMulSpacecraft();


    //
    //属性操作函数
    //
    inline void		SetName(const std::string& name);		// 设置航天器名称
    inline void		SetPos(const CCoord& pos);	            // 设置航天器位置
    inline void		SetVel(const CCoord& vel);	            // 设置航天器速度
    inline void		SetElapsedSec(double sec);				// 设置累计时间
    inline const std::string& GetName() const;			// 得到航天器名称
    inline const CCoord& GetPos() const;		        // 得到航天器位置
    inline const CCoord& GetVel() const;		        // 得到航天器速度
    inline double				GetElapsedSec() const;		// 得到累计时间

    void	Init();                                         // 仿真初始化
    void	TimeAdvance(double step, int burnCoordSys,
        const CCoord& burnValue);                       // 时间推进
    void    ReportGeneration();                             // 结果报告生成

private:
    //
    //保存中间数据
    //
    void	UpdateBuffer();

    //
    // Attribute.
    //
public:
    std::string		m_Name;				// 航天器名称
    CCoord		    m_Pos;		        // 航天器位置,地心惯性坐标系表示
    CCoord		    m_Vel;		        // 航天器速度,地心惯性坐标系表示
    double			m_ElapsedSec;		// 累计时间(s)


public:
    //
    //航天器数据缓冲区
    //
    struct CHistoryData
    {
        std::vector<double>		m_ElapsedSec;			// 仿真总共消耗时间[s]
        std::vector<CCoord>		m_Pos;				    // 位置状态[m]
        std::vector<CCoord>		m_Vel;				    // 速度状态[m/s]
    };
    CHistoryData	m_HistoryData;	    // 历史数据

protected:
    COrbit			m_Orbit;		    // 轨道积分类

};


//
//inline function.
//

// 设置航天器名称
void		CMulSpacecraft::SetName(const std::string& name)
{
    m_Name = name;
}

// 设置航天器状态
inline void		CMulSpacecraft::SetPos(const CCoord& pos)
{
    m_Pos = pos;
}

inline void		CMulSpacecraft::SetVel(const CCoord& vel)
{
    m_Vel = vel;
}

// 设置累计时间
inline void		CMulSpacecraft::SetElapsedSec(double sec)
{
    m_ElapsedSec = sec;
}

// 得到航天器名称
inline const std::string& CMulSpacecraft::GetName() const
{
    return m_Name;
}

// 得到航天器状态
inline const CCoord& CMulSpacecraft::GetPos() const
{
    return m_Pos;
}

inline const CCoord& CMulSpacecraft::GetVel() const
{
    return m_Vel;
}

// 得到累计时间
inline double					CMulSpacecraft::GetElapsedSec() const
{
    return m_ElapsedSec;
}




#endif // !defined(_MulSpacecraft_H__INCLUDED_)
