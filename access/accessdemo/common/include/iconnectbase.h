/********************************************************************
	created:	2017/10/27 16:14 
	file base:	iconnectbase.h
	author:		Milo
	
	purpose:	
*********************************************************************/
#ifndef _ICONNECT_BASE_H
#define _ICONNECT_BASE_H
#include "kdvtype.h"
#define NULL 0
enum EMICBErrcode
{
	EMICB_OK=0,                //返回成功
	EMICB_NO_SUPPORT_ERR=1,    //协议不支持该功能
	EMICB_PARAM_ERR=2,         //传递参数错误
	EMICB_NO_FOUND_INFO_ERR=3, //未找到对应的信息
	EMICB_FORBIDDEN_CLOSE = 4, //有未关闭的设备，禁止关闭连接
	EMICB_TIMEOUT = 5,
	EMICB_FAIL
};

//连接的协议类型
enum EMICBProtocolType
{
	EMICB_PROTO_Tcp = 0x0001,	         //TCP连接 ,TKDProtocolTCP有效
	EMICB_PROTO_Udp = 0x0002,	         //UDP连接
	EMICB_PROTO_Com = 0x0004,	         //串口连接
	EMICB_PROTO_TCPSERVER = 0x0008,      //作为TCP 服务器监听设备
};

//设备的连接状态
enum EMICBDevConnState {
	EMICB_DEV_CONNECT_IDLE,						//空闲
	EMICB_DEV_CONNECT_CONNECTED,				//已经连接
	EMICB_DEV_CONNECT_CONNECTING,				//正在连接
	EMICB_DEV_CONNECT_CONNECT_FAIL,				//连接失败
	EMICB_DEV_CONNECT_DISCONNECTED,				//已经断开
	EMICB_DEV_CONNECT_DISCONNECTING,			//正在断开
	EMICB_DEV_CONNECT_DISCONNECT_FAIL,			//断开失败
	EMICB_DEV_CONNECT_HARDWARE_DETECT_SUC,		//设备硬件检测成功
	EMICB_DEV_CONNECT_HARDWARE_DETECT_FAIL,		//设备硬件检测失败
};

//串口协议配置信息
typedef struct tagICBCOMCfg
{
	tagICBCOMCfg()
	{
		m_dwComport=0;
		m_dwAddress=0;
		m_emBaudrate=EBaud9600;
		m_emDatabits=EData8;
		m_emParity=EParNone;
		m_emStopbits=EStop1;
	}

	enum EMBaudrate
	{
		EBaud110 = 110,
		EBaud300 = 300,
		EBaud600 = 600,
		EBaud1200 = 1200,
		EBaud2400 = 2400,
		EBaud4800 = 4800,
		EBaud9600 = 9600,
		EBaud14400 = 14400,
		EBaud19200 = 19200,
		EBaud38400 = 38400,
		EBaud56000 = 56000,
		EBaud57600 = 57600,
		EBaud115200 = 115200,
		EBaud128000 = 128000,
		EBaud256000 = 256000
	};

	enum EMDataBits
	{
		EData5 = 5,
		EData6 = 6,
		EData7 = 7,
		EData8 = 8
	};

	enum EMParity
	{
		EParNone = 0,
		EParOdd = 1,
		EParEven = 2,
		EParMark = 3,
		EParSpace = 4
	};

	enum EMStopBits
	{
		EStop1 = 0,
		EStop1_5 = 1,
		EStop2 = 2
	};

	s32        m_dwComport;		           //串口号
	s32        m_dwAddress;                //串口地址(use in com485)
	EMBaudrate m_emBaudrate;	           //波特率
	EMDataBits m_emDatabits;	           //数据位
	EMParity   m_emParity;		           //校验位
	EMStopBits m_emStopbits;	           //停止位
}TICBCOMCfg, *PTICBCOMCfg;
#ifndef WIN32
__attribute__ ((packed))
#endif

//设备端IP结构定义
typedef struct tagICBAddress
{
	tagICBAddress(){m_dwIp=0;m_wPort=0;}
	u32 m_dwIp;     
	u16 m_wPort;

	bool friend operator<(const tagICBAddress &x,const tagICBAddress &y)  
	{  
		return x.m_dwIp < y.m_dwIp;
	}

}TICBAddress, *PTICBAddress;
#ifndef WIN32
__attribute__ ((packed))
#endif

typedef void (*pfICBConnectFunc)(u32 dwObjMark, EMICBDevConnState eDevState,void *pContext);

typedef void (*pfICBTranslateFunc)(u32 dwObjMark, const u8* pszData, s32 nLength,void *pContext);

/***************************************************************************/
/*接口调用顺序                                                             */
/*NewConnectObject(tcpserver和udp首先判断对应本地端口的连接是否已经建立，
已建立则不调用该接口，寻找之前创建的对象)
/*SetLocalCfg
/*GetLocalCfg（在SetLocalCfg之后，UnbindDeviceInfo之前可以选择性调用）
/*BindDeviceInfo
/*IsEqualDeviceInfo（在BindDeviceInfo之后和UnbindDeviceInfo之前可以选择性调用）
/*SetConnectStateCB（尽量在开启通信前设置回调）
/*SetTranslateCB（尽量在开启通信前设置回调）
/*StartConnect
/*SendData
/*UnbindDeviceInfo
/*StopConnect
/*DelConnectObject（tcpserver和udp首先确认目前无使用同一端口的连接再删除）
/***************************************************************************/
class IConnectBase
{
//供外部调用
public:
	IConnectBase(){};
	virtual ~IConnectBase() {};

public:
	/************************************************************************/
	/* 获取错误信息接口                                                     */
	/************************************************************************/
    virtual const s8 * GetLastErrStr() const = 0;
	virtual u32		GetLastErrNo() const = 0;

	/************************************************************************/
	/* 设置本地配置：                                                       */
	/* udp连接: ptLocalCfg为TICBAddress对象指针，IP和端口均要填写           */
	/*          dwLocalCfgSize 为TICBAddress对象的大小                      */
	/* tcp连接：ptLocalCfg为默认值NULL，dwLocalCfgSize为默认值0             */
	/* com连接：ptLocalCfg 填写TICBCOMCfg对象指针，                         */ 
	/*          dwLocalCfgSize 为TICBCOMCfg对象的大小                       */
	/* tcp server：ptLocalCfg为TICBAddress对象指针，仅填写端口即可          */
	/*             dwLocalCfgSize 为TICBAddress对象的大小                   */
	/* 返回值参考 EMICBErrcode                                              */
	/************************************************************************/
	virtual u32 SetLocalCfg(const void* ptLocalCfg= NULL, u32 dwLocalCfgSize=0) = 0;

	/************************************************************************/
	/* 获取本地配置：                                                       */
	/* udp连接: ptLocalCfg为TICBAddress对象指针，返回信息IP和端口均有效     */
	/*          dwLocalCfgSize 为TICBAddress对象的大小                      */
	/* tcp连接：该接口无效                                                  */
	/* com连接：ptLocalCfg 填写TICBCOMCfg对象指针，                         */ 
	/*          dwLocalCfgSize 为TICBCOMCfg对象的大小                       */
	/* tcp server：ptLocalCfg为TICBAddress对象指针，返回信息仅端口有效      */
	/*             dwLocalCfgSize 为TICBAddress对象的大小                   */
	/* 返回值参考 EMICBErrcode                                              */
	/************************************************************************/
	virtual u32 GetLocalCfg(void *ptLocalCfg=NULL) = 0;

	/************************************************************************/
	/* 绑定连接设备的信息：                                                 */
	/* dwObjMark:为使用该连接对象的标识，可以根据实际使用时进行传递，在连接 */
	/* 回调和数据回调中会携带该信息                                         */
	/* udp连接: ptDevAddr需要填写设备端配置的IP和端口                       */
	/* tcp连接：ptDevAddr需要填写设备端配置的IP和端口                       */
	/* com连接：ptDevAddr 该值继续保持默认值为空                            */
	/* tcp server：ptDevAddr 中仅需要填写对端的IP即可                       */
	/* 返回值参考 EMICBErrcode                                              */
	/************************************************************************/
	virtual u32 BindDeviceInfo(u32 dwObjMark,const TICBAddress* ptDevAddr = NULL) = 0;

	/************************************************************************/
	/* 解绑定连接设备的信息：                                               */
	/* dwObjMark:为使用该连接对象的标识，可以根据实际使用时进行传递，在连接 */
	/* 回调和数据回调中会携带该信息                                         */
	/* udp连接: ptDevAddr需要填写设备端配置的IP和端口                       */
	/* tcp连接：ptDevAddr需要填写设备端配置的IP和端口                       */
	/* com连接：ptDevAddr 该值继续保持默认值为空                            */
	/* tcp server：ptDevAddr 中仅需要填写对端的IP即可                       */
	/* 返回值参考 EMICBErrcode                                              */
	/************************************************************************/
	virtual u32 UnbindDeviceInfo(u32 dwObjMark, const TICBAddress* ptDevAddr=NULL ) = 0;

	/************************************************************************/
	/* 解绑定连接设备的信息：                                               */
	/* udp连接: ptDevAddr需要填写设备端配置的IP和端口                       */
	/* tcp连接：ptDevAddr需要填写设备端配置的IP和端口                       */
	/* com连接：该接口调用无效                                              */
	/* tcp server：ptDevAddr 中仅需要填写对端的IP即可                       */
	/************************************************************************/
	virtual BOOL32 IsEqualDeviceInfo(const TICBAddress* ptDevAddr)=0;

	/************************************************************************/
	/* 开启连接                                                             */
	/* 返回值参考 EMICBErrcode                                              
	/************************************************************************/
	virtual u32 StartConnect(void)=0;

	/************************************************************************/
	/* 停止连接                                                             */
	/* 返回值参考 EMICBErrcode                                              */
	/************************************************************************/
	virtual u32 StopConnect() = 0;

	/************************************************************************/
	/* 向连接设备发送数据的接口(异步)                                       */
	/* dwObjMark:为使用该连接对象的标识，与BindDeviceInfo接口中传递值相同   */
	/* pszData:数据值指针                                                   */
	/* nLength:数据长度                                                     */
	/* dwTimeOut:数据传递的超时时间                                         */ 
	/* 返回值参考 EMICBErrcode                                              */
	/************************************************************************/
	virtual u32 PostData(u32 dwObjMark, const u8* pszData, s32 nLength, u32 dwTimeOut = 0) = 0;
	/************************************************************************/
	/* 向连接设备发送数据的接口(同步)                                       */
	/* dwObjMark:为使用该连接对象的标识，与BindDeviceInfo接口中传递值相同   */
	/* pszData:数据值指针                                                   */
	/* nLength:数据长度                                                     */
	/* dwTimeOut:数据传递的超时时间                                         */ 
	/* 对于udp连接无效                                                       */
	/* 返回值参考 EMICBErrcode                                              */
	/************************************************************************/
	virtual u32 SendData(u32 dwObjMark, const u8* pszData, s32 nLength, u32 dwTimeOut = 0) = 0;

	/************************************************************************/
	/* 设置连接状态的回调                                                   */
	/* cb:为状态回调函数                                                    */
	/* pContext为设置参数，回调函数传出                                     */
	/************************************************************************/
    virtual void SetConnectStateCB(pfICBConnectFunc cb,void *pContext=NULL) = 0;

	/************************************************************************/
	/* 设置数据信息的回调                                                   */
	/* cb:为数据信息回调函数                                                */
	/* pContext为设置参数，回调函数传出                                     */
	/************************************************************************/
	virtual void SetTranslateCB(pfICBTranslateFunc cb,void *pContext=NULL) = 0;
};

/*=================================================================
函数名    :  NewConnectObject
功  能    :  创建连接对象
算法实现  : 
参数说明  :  EMKDIProtocolType eType：连接类型
返回值说明:
注  明    :  
===================================================================*/
IConnectBase * NewConnectObject(EMICBProtocolType eType);

/*=================================================================
函数名    :  DelConnectObject
功  能    :  删除连接对象
算法实现  : 
参数说明  :  IConnectBase *pConnectBase：NewConnectObject创建的连接对象
返回值说明:
注  明    :  
===================================================================*/
void DelConnectObject(IConnectBase *pConnectBase);

#endif

