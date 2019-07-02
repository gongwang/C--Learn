

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sun Jul 29 12:08:22 2018
 */
/* Compiler settings for svrmwplayer.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_svrmwplayerLib,0xE0F6C8A5,0x1369,0x4A1E,0x86,0x37,0xD7,0x58,0x8B,0xC0,0x6D,0xA2);


MIDL_DEFINE_GUID(IID, DIID__Dsvrmwplayer,0xD3AB8013,0x7AED,0x46CC,0xB0,0xEE,0x2B,0x94,0x9B,0xEB,0x15,0xEA);


MIDL_DEFINE_GUID(IID, DIID__DsvrmwplayerEvents,0xA0D18D48,0x22EB,0x4A76,0xAA,0xAA,0x1A,0xB6,0x4E,0x78,0x93,0x20);


MIDL_DEFINE_GUID(CLSID, CLSID_svrmwplayer,0xE804F18D,0x1C55,0x487E,0xB7,0xF5,0x46,0x69,0x02,0x6F,0x8C,0x61);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



