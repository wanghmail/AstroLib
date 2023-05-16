// ExampleOrbElemConv.cpp: implementation of the ExampleOrbElemConv class.
//
//////////////////////////////////////////////////////////////////////

#include "AstroLibExamples.h"
#include "ExampleOrbElemConv.h"



//********************************************************************
/// 轨道参数集转换例子.
/// @Author	Wang Hua
/// @Date	2008.8.8
//********************************************************************
void ExampleOrbElemConv()
{
    double            gm = AsCEarthGrav; // gravitational parameter 
    CModOrbElem       modOrbElems;
    CCoord            position,
                      velocity;

    // Initialize the modified orbit elements for inertial frame 

    modOrbElems.m_PeriRad = 6678137.0;
    modOrbElems.m_Ecc     = 0.0;
    modOrbElems.m_I       = AsDegToRad(28.5);
    modOrbElems.m_RAAN    = 0.0;
    modOrbElems.m_ArgPeri = 0.0;
    modOrbElems.m_TrueA   = 0.0;

    // Convert modified orbit elements to cartesian 

    AsModOrbElemToCart(modOrbElems, gm, position, velocity);
    printf("Cartesian elements:\n");
    printf("x   = %10.6f\n", position[0]);
    printf("y   = %10.6f\n", position[1]);
    printf("z   = %10.6f\n", position[2]);
    printf("xdot= %10.6f\n", velocity[0]);
    printf("ydot= %10.6f\n", velocity[1]);
    printf("zdot= %10.6f\n\n", velocity[2]);

}
  