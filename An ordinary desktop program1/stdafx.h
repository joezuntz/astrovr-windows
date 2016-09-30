//#include <afxcontrolbars.h>
//#include <afxcontrolbars.h>


#if _MSC_VER > 1000
#pragma once
#endif

#ifndef STRICT
#define STRICT
#endif

#ifndef WINVER
#define WINVER _WIN32_WINNT_WIN10
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT _WIN32_WINNT_WIN10
#endif						

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS _WIN32_WINNT_WIN10
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0400
#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#define _AFX_ALL_WARNINGS // turns off MFC's hiding of some common and often safely ignored warning messages

//#define AAPLUS_VSOP87_NO_HIGH_PRECISION //Uncomment this to test out excluding support for full VSOP87 support

#ifndef _SECURE_ATL
#define _SECURE_ATL 1 //Use the Secure C Runtime in ATL
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif


#define JOE_NONSENSE 1

