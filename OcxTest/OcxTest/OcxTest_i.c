

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon Jul 23 15:04:07 2018
 */
/* Compiler settings for OcxTest.idl:
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

MIDL_DEFINE_GUID(IID, LIBID_OcxTestLib,0xE549B9D8,0x8A72,0x4D03,0x87,0x47,0x7C,0x62,0x8E,0x8B,0x93,0x64);


MIDL_DEFINE_GUID(IID, DIID__DOcxTest,0x905E7170,0x806B,0x477C,0x93,0xE9,0xA6,0x5A,0x1F,0xC4,0x7E,0xAF);


MIDL_DEFINE_GUID(IID, DIID__DOcxTestEvents,0x56AFC954,0x4EEB,0x43B4,0x92,0xCF,0x24,0x41,0xBE,0x36,0xFA,0x6B);


MIDL_DEFINE_GUID(CLSID, CLSID_OcxTest,0x92F94649,0x535A,0x4C1E,0xBE,0xB8,0xA4,0xF7,0xC2,0xEF,0xDD,0x26);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



