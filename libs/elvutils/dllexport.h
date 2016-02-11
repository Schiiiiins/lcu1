#ifndef _ELVUTILS_DLLEXPORT_H_
#define _ELVUTILS_DLLEXPORT_H_

#ifdef WIN32
  #ifdef ELVUTILS_BUILD_DLL
    #define ELVUTILS_DLLEXPORT __declspec(dllexport)
    #pragma warning( disable: 4251 )
  #else
    #ifdef BUILD_LIB
      #define ELVUTILS_DLLEXPORT
    #else
      #define ELVUTILS_DLLEXPORT __declspec(dllimport)
      #pragma warning( disable: 4251 )
    #endif
  #endif
#else
  #define ELVUTILS_DLLEXPORT
#endif

#endif

