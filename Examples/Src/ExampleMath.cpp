// ExampleMath.cpp: implementation of the ExampleMath class.
//
//////////////////////////////////////////////////////////////////////

#include "AstroLibExamples.h"
#include "ExampleMath.h"


//********************************************************************
/// The purpose of this program is to provide an example
///   that illustrates general math operations.
/// Functions Used: AsDegToRad()
///                 AsMax()
///                 AsMin()
///                 AsRound()
/// @Author	Wang Hua
/// @Date	2008.8.8
//********************************************************************
void ExampleMath()
{
    double          angle1, 
                    angle2;                 

    printf("========================================================\n");
    printf("Initial angles in degrees...\n"); 
    printf("========================================================\n");
    angle1 = 337.5;
    angle2 = 270.0;
    printf ("Angle1 (deg) = %.3f\n",    angle1);
    printf ("Angle2 (deg) = %.3f\n\n",  angle2);
    
    printf("========================================================\n");
    printf("Convert angles to radians...\n"); 
    printf("========================================================\n");
    printf ("Angle1 (rad) = %.3f\n",   AsDegToRad(angle1));
    printf ("Angle2 (rad) = %.3f\n\n", AsDegToRad(angle2));

    printf("========================================================\n");
    printf("Determine the Maximum of the two angles...\n"); 
    printf("========================================================\n");
    printf ("Angle1  = %.3f\n",      angle1);
    printf ("Angle2  = %.3f\n",      angle2);
    printf ("Maximum = %.3f\n\n",  AsMax(angle1,  angle2));

    printf("========================================================\n");
    printf("Determine the Minimum of the two angles...\n"); 
    printf("========================================================\n");
    printf ("Angle1  = %.3f\n",      angle1);
    printf ("Angle2  = %.3f\n",      angle2);
    printf ("Minimum = %.3f\n\n",  AsMin(angle1,  angle2));

    printf("========================================================\n");
    printf("Round off Angle1 to the nearest whole degree...\n"); 
    printf("========================================================\n");
    printf ("Angle1  = %.3f\n",  angle1);
    printf ("Rounded = %.3f\n\n",  AsRound(angle1));
}


