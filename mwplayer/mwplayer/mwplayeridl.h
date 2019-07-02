

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __mwplayeridl_h__
#define __mwplayeridl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___Dmwplayer_FWD_DEFINED__
#define ___Dmwplayer_FWD_DEFINED__
typedef interface _Dmwplayer _Dmwplayer;
#endif 	/* ___Dmwplayer_FWD_DEFINED__ */


#ifndef ___DmwplayerEvents_FWD_DEFINED__
#define ___DmwplayerEvents_FWD_DEFINED__
typedef interface _DmwplayerEvents _DmwplayerEvents;
#endif 	/* ___DmwplayerEvents_FWD_DEFINED__ */


#ifndef __mwplayer_FWD_DEFINED__
#define __mwplayer_FWD_DEFINED__

#ifdef __cplusplus
typedef class mwplayer mwplayer;
#else
typedef struct mwplayer mwplayer;
#endif /* __cplusplus */

#endif 	/* __mwplayer_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __mwplayerLib_LIBRARY_DEFINED__
#define __mwplayerLib_LIBRARY_DEFINED__

/* library mwplayerLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_mwplayerLib;

#ifndef ___Dmwplayer_DISPINTERFACE_DEFINED__
#define ___Dmwplayer_DISPINTERFACE_DEFINED__

/* dispinterface _Dmwplayer */
/* [uuid] */ 


EXTERN_C const IID DIID__Dmwplayer;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("3681B6D3-533D-4D68-8EC7-6A2A020448D4")
    _Dmwplayer : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DmwplayerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _Dmwplayer * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _Dmwplayer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _Dmwplayer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _Dmwplayer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _Dmwplayer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _Dmwplayer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _Dmwplayer * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DmwplayerVtbl;

    interface _Dmwplayer
    {
        CONST_VTBL struct _DmwplayerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _Dmwplayer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _Dmwplayer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _Dmwplayer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _Dmwplayer_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _Dmwplayer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _Dmwplayer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _Dmwplayer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___Dmwplayer_DISPINTERFACE_DEFINED__ */


#ifndef ___DmwplayerEvents_DISPINTERFACE_DEFINED__
#define ___DmwplayerEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DmwplayerEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__DmwplayerEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("FA246079-4248-48E4-AA26-AA1534372437")
    _DmwplayerEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DmwplayerEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DmwplayerEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DmwplayerEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DmwplayerEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DmwplayerEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DmwplayerEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DmwplayerEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DmwplayerEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DmwplayerEventsVtbl;

    interface _DmwplayerEvents
    {
        CONST_VTBL struct _DmwplayerEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DmwplayerEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DmwplayerEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DmwplayerEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DmwplayerEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DmwplayerEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DmwplayerEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DmwplayerEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DmwplayerEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_mwplayer;

#ifdef __cplusplus

class DECLSPEC_UUID("A9E2C003-1BE3-40AA-BA1B-77B787E381CC")
mwplayer;
#endif
#endif /* __mwplayerLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


