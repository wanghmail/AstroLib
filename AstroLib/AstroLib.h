////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2010                          //
//                              Wang Hua                                  //
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
#ifdef _DEBUG
	#pragma comment(lib, "./Lib/AstroLibD.lib")
	#pragma message("AstroLib (Debug "AstroLib_Version") has been successfully linked.")
#else
	#pragma comment(lib, "./Lib/AstroLib.lib")
	#pragma message("AstroLib ("AstroLib_Version") has been successfully linked.")
#endif
#ifndef __cplusplus
	#error AstroLib requires C++ compilation
#endif
//#ifndef _AFXDLL
//	#error AstroLib requires use MFC in a shared DLL.
//#endif
#pragma warning( disable : 4786 )  // Disable warning messages


//
//Include file.
//
#include "./Include/AsAttitudeParam.h"
#include "./Include/AsCommon.h"
#include "./Include/AsCoordinate.h"
#include "./Include/AsDResolve.h"
#include "./Include/AsInterpolation.h"
#include "./Include/AsMatrix.h"
#include "./Include/AsMath.h"
#include "./Include/AsOrbitParam.h"
#include "./Include/AsRandom.h"
#include "./Include/AsRightFunction.h"
#include "./Include/AsTimeSystem.h"
#include "./Include/AsVector.h"


AsMUsingNamespace


#endif //_ASTROLIB_H_


////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2008                          //
//                              Wang Hua                                  //
//             College of Aerospace and Material Engineering              //
//               National University of Defense Technology                //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

