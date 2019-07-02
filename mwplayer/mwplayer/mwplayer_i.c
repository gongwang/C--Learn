

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sun Jul 29 11:39:41 2018
 */
/* Compiler settings for mwplayer.idl:
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

MIDL_DEFINE_GUID(IID, LIBID_mwplayerLib,0x2BF9D73A,0x294B,0x488A,0xB1,0xD3,0x49,0x13,0x08,0xF1,0xC8,0x06);


MIDL_DEFINE_GUID(IID, DIID__Dmwplayer,0x3681B6D3,0x533D,0x4D68,0x8E,0xC7,0x6A,0x2A,0x02,0x04,0x48,0xD4);


MIDL_DEFINE_GUID(IID, DIID__DmwplayerEvents,0xFA246079,0x4248,0x48E4,0xAA,0x26,0xAA,0x15,0x34,0x37,0x24,0x37);


MIDL_DEFINE_GUID(CLSID, CLSID_mwplayer,0xA9E2C003,0x1BE3,0x40AA,0xBA,0x1B,0x77,0xB7,0x87,0xE3,0x81,0xCC);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



