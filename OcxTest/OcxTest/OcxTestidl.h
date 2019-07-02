

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __OcxTestidl_h__
#define __OcxTestidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DOcxTest_FWD_DEFINED__
#define ___DOcxTest_FWD_DEFINED__
typedef interface _DOcxTest _DOcxTest;
#endif 	/* ___DOcxTest_FWD_DEFINED__ */


#ifndef ___DOcxTestEvents_FWD_DEFINED__
#define ___DOcxTestEvents_FWD_DEFINED__
typedef interface _DOcxTestEvents _DOcxTestEvents;
#endif 	/* ___DOcxTestEvents_FWD_DEFINED__ */


#ifndef __OcxTest_FWD_DEFINED__
#define __OcxTest_FWD_DEFINED__

#ifdef __cplusplus
typedef class OcxTest OcxTest;
#else
typedef struct OcxTest OcxTest;
#endif /* __cplusplus */

#endif 	/* __OcxTest_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __OcxTestLib_LIBRARY_DEFINED__
#define __OcxTestLib_LIBRARY_DEFINED__

/* library OcxTestLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_OcxTestLib;

#ifndef ___DOcxTest_DISPINTERFACE_DEFINED__
#define ___DOcxTest_DISPINTERFACE_DEFINED__

/* dispinterface _DOcxTest */
/* [uuid] */ 


EXTERN_C const IID DIID__DOcxTest;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("905E7170-806B-477C-93E9-A65A1FC47EAF")
    _DOcxTest : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DOcxTestVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DOcxTest * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DOcxTest * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DOcxTest * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DOcxTest * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DOcxTest * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DOcxTest * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DOcxTest * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DOcxTestVtbl;

    interface _DOcxTest
    {
        CONST_VTBL struct _DOcxTestVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DOcxTest_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DOcxTest_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DOcxTest_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DOcxTest_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DOcxTest_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DOcxTest_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DOcxTest_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DOcxTest_DISPINTERFACE_DEFINED__ */


#ifndef ___DOcxTestEvents_DISPINTERFACE_DEFINED__
#define ___DOcxTestEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DOcxTestEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__DOcxTestEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("56AFC954-4EEB-43B4-92CF-2441BE36FA6B")
    _DOcxTestEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DOcxTestEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DOcxTestEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DOcxTestEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DOcxTestEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DOcxTestEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DOcxTestEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DOcxTestEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DOcxTestEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DOcxTestEventsVtbl;

    interface _DOcxTestEvents
    {
        CONST_VTBL struct _DOcxTestEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DOcxTestEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DOcxTestEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DOcxTestEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DOcxTestEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DOcxTestEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DOcxTestEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DOcxTestEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DOcxTestEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_OcxTest;

#ifdef __cplusplus

class DECLSPEC_UUID("92F94649-535A-4C1E-BEB8-A4F7C2EFDD26")
OcxTest;
#endif
#endif /* __OcxTestLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


