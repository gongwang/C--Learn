

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __svrmwplayeridl_h__
#define __svrmwplayeridl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___Dsvrmwplayer_FWD_DEFINED__
#define ___Dsvrmwplayer_FWD_DEFINED__
typedef interface _Dsvrmwplayer _Dsvrmwplayer;
#endif 	/* ___Dsvrmwplayer_FWD_DEFINED__ */


#ifndef ___DsvrmwplayerEvents_FWD_DEFINED__
#define ___DsvrmwplayerEvents_FWD_DEFINED__
typedef interface _DsvrmwplayerEvents _DsvrmwplayerEvents;
#endif 	/* ___DsvrmwplayerEvents_FWD_DEFINED__ */


#ifndef __svrmwplayer_FWD_DEFINED__
#define __svrmwplayer_FWD_DEFINED__

#ifdef __cplusplus
typedef class svrmwplayer svrmwplayer;
#else
typedef struct svrmwplayer svrmwplayer;
#endif /* __cplusplus */

#endif 	/* __svrmwplayer_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __svrmwplayerLib_LIBRARY_DEFINED__
#define __svrmwplayerLib_LIBRARY_DEFINED__

/* library svrmwplayerLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_svrmwplayerLib;

#ifndef ___Dsvrmwplayer_DISPINTERFACE_DEFINED__
#define ___Dsvrmwplayer_DISPINTERFACE_DEFINED__

/* dispinterface _Dsvrmwplayer */
/* [uuid] */ 


EXTERN_C const IID DIID__Dsvrmwplayer;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("D3AB8013-7AED-46CC-B0EE-2B949BEB15EA")
    _Dsvrmwplayer : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DsvrmwplayerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _Dsvrmwplayer * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _Dsvrmwplayer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _Dsvrmwplayer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _Dsvrmwplayer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _Dsvrmwplayer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _Dsvrmwplayer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _Dsvrmwplayer * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DsvrmwplayerVtbl;

    interface _Dsvrmwplayer
    {
        CONST_VTBL struct _DsvrmwplayerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _Dsvrmwplayer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _Dsvrmwplayer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _Dsvrmwplayer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _Dsvrmwplayer_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _Dsvrmwplayer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _Dsvrmwplayer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _Dsvrmwplayer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___Dsvrmwplayer_DISPINTERFACE_DEFINED__ */


#ifndef ___DsvrmwplayerEvents_DISPINTERFACE_DEFINED__
#define ___DsvrmwplayerEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DsvrmwplayerEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__DsvrmwplayerEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("A0D18D48-22EB-4A76-AAAA-1AB64E789320")
    _DsvrmwplayerEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DsvrmwplayerEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DsvrmwplayerEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DsvrmwplayerEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DsvrmwplayerEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DsvrmwplayerEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DsvrmwplayerEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DsvrmwplayerEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DsvrmwplayerEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DsvrmwplayerEventsVtbl;

    interface _DsvrmwplayerEvents
    {
        CONST_VTBL struct _DsvrmwplayerEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DsvrmwplayerEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DsvrmwplayerEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DsvrmwplayerEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DsvrmwplayerEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DsvrmwplayerEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DsvrmwplayerEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DsvrmwplayerEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DsvrmwplayerEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_svrmwplayer;

#ifdef __cplusplus

class DECLSPEC_UUID("E804F18D-1C55-487E-B7F5-4669026F8C61")
svrmwplayer;
#endif
#endif /* __svrmwplayerLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


