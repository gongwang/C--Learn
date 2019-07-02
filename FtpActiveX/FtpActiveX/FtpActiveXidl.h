

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __FtpActiveXidl_h__
#define __FtpActiveXidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DFtpActiveX_FWD_DEFINED__
#define ___DFtpActiveX_FWD_DEFINED__
typedef interface _DFtpActiveX _DFtpActiveX;
#endif 	/* ___DFtpActiveX_FWD_DEFINED__ */


#ifndef ___DFtpActiveXEvents_FWD_DEFINED__
#define ___DFtpActiveXEvents_FWD_DEFINED__
typedef interface _DFtpActiveXEvents _DFtpActiveXEvents;
#endif 	/* ___DFtpActiveXEvents_FWD_DEFINED__ */


#ifndef __FtpActiveX_FWD_DEFINED__
#define __FtpActiveX_FWD_DEFINED__

#ifdef __cplusplus
typedef class FtpActiveX FtpActiveX;
#else
typedef struct FtpActiveX FtpActiveX;
#endif /* __cplusplus */

#endif 	/* __FtpActiveX_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __FtpActiveXLib_LIBRARY_DEFINED__
#define __FtpActiveXLib_LIBRARY_DEFINED__

/* library FtpActiveXLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_FtpActiveXLib;

#ifndef ___DFtpActiveX_DISPINTERFACE_DEFINED__
#define ___DFtpActiveX_DISPINTERFACE_DEFINED__

/* dispinterface _DFtpActiveX */
/* [uuid] */ 


EXTERN_C const IID DIID__DFtpActiveX;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("CDEF82FB-13BE-4B83-B13C-0CA2562CF16E")
    _DFtpActiveX : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DFtpActiveXVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DFtpActiveX * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DFtpActiveX * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DFtpActiveX * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DFtpActiveX * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DFtpActiveX * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DFtpActiveX * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DFtpActiveX * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DFtpActiveXVtbl;

    interface _DFtpActiveX
    {
        CONST_VTBL struct _DFtpActiveXVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DFtpActiveX_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DFtpActiveX_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DFtpActiveX_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DFtpActiveX_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DFtpActiveX_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DFtpActiveX_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DFtpActiveX_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DFtpActiveX_DISPINTERFACE_DEFINED__ */


#ifndef ___DFtpActiveXEvents_DISPINTERFACE_DEFINED__
#define ___DFtpActiveXEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DFtpActiveXEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__DFtpActiveXEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("C13A54AB-5E27-47D4-B899-8E865B20A592")
    _DFtpActiveXEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DFtpActiveXEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DFtpActiveXEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DFtpActiveXEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DFtpActiveXEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DFtpActiveXEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DFtpActiveXEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DFtpActiveXEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DFtpActiveXEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DFtpActiveXEventsVtbl;

    interface _DFtpActiveXEvents
    {
        CONST_VTBL struct _DFtpActiveXEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DFtpActiveXEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DFtpActiveXEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DFtpActiveXEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DFtpActiveXEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DFtpActiveXEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DFtpActiveXEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DFtpActiveXEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DFtpActiveXEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_FtpActiveX;

#ifdef __cplusplus

class DECLSPEC_UUID("F770ACF3-7BD1-4D3A-B57D-52376D6189D3")
FtpActiveX;
#endif
#endif /* __FtpActiveXLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


