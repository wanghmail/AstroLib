////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2022                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                   程序或文档中有任何问题，欢迎告知。                      //
//                             13973104555                                //
//                         wanghmail@qq.com                               //
//                             QQ: 3062881                                //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

//
//include head file of AstroLib.
//

#if !defined (_ASTROLIB_H_)
#define  _ASTROLIB_H_


//
//Version declaration.
//
#define AstroLib_Version   "Version 0.11"
#define AstroLib_Copyright "Copyright 2003-2010, National University of Defense Technology."


//
//Library.
//
#if defined(_WIN64) && defined(_DEBUG)
#	pragma comment(lib, "./Lib/AstroLibD64.lib")
#	pragma message("AstroLib (Debug Version 1.0) has been successfully linked.")
#	define WIN32_LEAN_AND_MEAN
#elif defined(_WIN64)
#	pragma comment(lib, "./Lib/AstroLib64.lib")
#	pragma message("AstroLib (Release Version 1.0) has been successfully linked.")
#	define WIN32_LEAN_AND_MEAN
#elif defined(WIN32) && defined(_DEBUG)
#	pragma comment(lib, "./Lib/AstroLibD.lib")
#	pragma message("AstroLib (Debug Version 1.0) has been successfully linked.")
#	define WIN32_LEAN_AND_MEAN
#elif defined(WIN32)
#	pragma comment(lib, "./Lib/AstroLib.lib")
#	pragma message("AstroLib (Release Version 1.0) has been successfully linked.")
#	define WIN32_LEAN_AND_MEAN
#endif
#pragma warning( disable : 4786 )  // Disable warning messages


//
//Include file.
//
#include "./Include/AsAtmosphere.h"
#include "./Include/AsAttitude.h"
#include "./Include/AsAttitudeParam.h"
#include "./Include/AsCommon.h"
#include "./Include/AsCoordinate.h"
#include "./Include/AsDResolve.h"
#include "./Include/AsInterpolation.h"
#include "./Include/AsMatrix.h"
#include "./Include/AsMath.h"
#include "./Include/AsOrbit.h"
#include "./Include/AsOrbitParam.h"
#include "./Include/AsPlanetProp.h"
#include "./Include/AsRandom.h"
#include "./Include/AsRightFunction.h"
#include "./Include/AsTimeSystem.h"
#include "./Include/AsVector.h"
#include "./Include/AsAttitudeParam_2023.h"
#include "./Include/AsCoordinate_2023.h"
#include "./Include/AsCoordSystem_2023.h"
#include "./Include/AsOrbitParam_2023.h"





#endif //_ASTROLIB_H_


////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2022                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

