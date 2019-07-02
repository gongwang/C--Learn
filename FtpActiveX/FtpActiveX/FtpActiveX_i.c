

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Mar 01 16:29:36 2019
 */
/* Compiler settings for FtpActiveX.idl:
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

MIDL_DEFINE_GUID(IID, LIBID_FtpActiveXLib,0x36407BDF,0xD73C,0x4A0E,0xA1,0xD9,0xC4,0x57,0x4F,0x84,0xB8,0x9A);


MIDL_DEFINE_GUID(IID, DIID__DFtpActiveX,0xCDEF82FB,0x13BE,0x4B83,0xB1,0x3C,0x0C,0xA2,0x56,0x2C,0xF1,0x6E);


MIDL_DEFINE_GUID(IID, DIID__DFtpActiveXEvents,0xC13A54AB,0x5E27,0x47D4,0xB8,0x99,0x8E,0x86,0x5B,0x20,0xA5,0x92);


MIDL_DEFINE_GUID(CLSID, CLSID_FtpActiveX,0xF770ACF3,0x7BD1,0x4D3A,0xB5,0x7D,0x52,0x37,0x6D,0x61,0x89,0xD3);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



