

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __onlytestidl_h__
#define __onlytestidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___Donlytest_FWD_DEFINED__
#define ___Donlytest_FWD_DEFINED__
typedef interface _Donlytest _Donlytest;
#endif 	/* ___Donlytest_FWD_DEFINED__ */


#ifndef ___DonlytestEvents_FWD_DEFINED__
#define ___DonlytestEvents_FWD_DEFINED__
typedef interface _DonlytestEvents _DonlytestEvents;
#endif 	/* ___DonlytestEvents_FWD_DEFINED__ */


#ifndef __onlytest_FWD_DEFINED__
#define __onlytest_FWD_DEFINED__

#ifdef __cplusplus
typedef class onlytest onlytest;
#else
typedef struct onlytest onlytest;
#endif /* __cplusplus */

#endif 	/* __onlytest_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __onlytestLib_LIBRARY_DEFINED__
#define __onlytestLib_LIBRARY_DEFINED__

/* library onlytestLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_onlytestLib;

#ifndef ___Donlytest_DISPINTERFACE_DEFINED__
#define ___Donlytest_DISPINTERFACE_DEFINED__

/* dispinterface _Donlytest */
/* [uuid] */ 


EXTERN_C const IID DIID__Donlytest;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("22DCBE03-7163-43C2-9916-9847B73556AE")
    _Donlytest : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DonlytestVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _Donlytest * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _Donlytest * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _Donlytest * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _Donlytest * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _Donlytest * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _Donlytest * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _Donlytest * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DonlytestVtbl;

    interface _Donlytest
    {
        CONST_VTBL struct _DonlytestVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _Donlytest_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _Donlytest_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _Donlytest_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _Donlytest_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _Donlytest_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _Donlytest_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _Donlytest_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___Donlytest_DISPINTERFACE_DEFINED__ */


#ifndef ___DonlytestEvents_DISPINTERFACE_DEFINED__
#define ___DonlytestEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DonlytestEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__DonlytestEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("14BB9456-FF57-4462-9BF3-5C67347BA6ED")
    _DonlytestEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DonlytestEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DonlytestEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DonlytestEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DonlytestEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DonlytestEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DonlytestEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DonlytestEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DonlytestEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DonlytestEventsVtbl;

    interface _DonlytestEvents
    {
        CONST_VTBL struct _DonlytestEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DonlytestEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DonlytestEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DonlytestEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DonlytestEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DonlytestEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DonlytestEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DonlytestEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DonlytestEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_onlytest;

#ifdef __cplusplus

class DECLSPEC_UUID("15F63DED-B7C5-42BA-AD72-F5D8364B2B47")
onlytest;
#endif
#endif /* __onlytestLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


