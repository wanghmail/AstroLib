////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                       Copyright (c) 2003-2008                          //
//                              Wang Hua                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// AsCommon.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(_ASCOMMON_H_)
#define _ASCOMMON_H_


//#define __AS_NAMESPACE	//Namespace
//#define __AS_DLL			//Dynamic-link library




#if defined(__AS_NAMESPACE)
#  define AsMBeginNamespace namespace AS {
#  define AsMEndNamespace   }
#  define AsMNamespace      AS::
#  define AsMUsingNamespace using namespace AS;
#else
#  define AsMBeginNamespace
#  define AsMEndNamespace
#  define AsMNamespace
#  define AsMUsingNamespace
#endif


AsMBeginNamespace


typedef double DP;


AsMEndNamespace


#endif // !defined(_ASCOMMON_H_)
