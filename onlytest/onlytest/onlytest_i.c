

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Tue May 14 20:20:18 2019
 */
/* Compiler settings for onlytest.idl:
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

MIDL_DEFINE_GUID(IID, LIBID_onlytestLib,0xFAE33989,0x6CA1,0x47B7,0xBB,0xBD,0x60,0x89,0x0A,0x9B,0x34,0xE8);


MIDL_DEFINE_GUID(IID, DIID__Donlytest,0x22DCBE03,0x7163,0x43C2,0x99,0x16,0x98,0x47,0xB7,0x35,0x56,0xAE);


MIDL_DEFINE_GUID(IID, DIID__DonlytestEvents,0x14BB9456,0xFF57,0x4462,0x9B,0xF3,0x5C,0x67,0x34,0x7B,0xA6,0xED);


MIDL_DEFINE_GUID(CLSID, CLSID_onlytest,0x15F63DED,0xB7C5,0x42BA,0xAD,0x72,0xF5,0xD8,0x36,0x4B,0x2B,0x47);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



