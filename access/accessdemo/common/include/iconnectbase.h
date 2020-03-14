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
	EMICB_OK=0,                //���سɹ�
	EMICB_NO_SUPPORT_ERR=1,    //Э�鲻֧�ָù���
	EMICB_PARAM_ERR=2,         //���ݲ�������
	EMICB_NO_FOUND_INFO_ERR=3, //δ�ҵ���Ӧ����Ϣ
	EMICB_FORBIDDEN_CLOSE = 4, //��δ�رյ��豸����ֹ�ر�����
	EMICB_TIMEOUT = 5,
	EMICB_FAIL
};

//���ӵ�Э������
enum EMICBProtocolType
{
	EMICB_PROTO_Tcp = 0x0001,	         //TCP���� ,TKDProtocolTCP��Ч
	EMICB_PROTO_Udp = 0x0002,	         //UDP����
	EMICB_PROTO_Com = 0x0004,	         //��������
	EMICB_PROTO_TCPSERVER = 0x0008,      //��ΪTCP �����������豸
};

//�豸������״̬
enum EMICBDevConnState {
	EMICB_DEV_CONNECT_IDLE,						//����
	EMICB_DEV_CONNECT_CONNECTED,				//�Ѿ�����
	EMICB_DEV_CONNECT_CONNECTING,				//��������
	EMICB_DEV_CONNECT_CONNECT_FAIL,				//����ʧ��
	EMICB_DEV_CONNECT_DISCONNECTED,				//�Ѿ��Ͽ�
	EMICB_DEV_CONNECT_DISCONNECTING,			//���ڶϿ�
	EMICB_DEV_CONNECT_DISCONNECT_FAIL,			//�Ͽ�ʧ��
	EMICB_DEV_CONNECT_HARDWARE_DETECT_SUC,		//�豸Ӳ�����ɹ�
	EMICB_DEV_CONNECT_HARDWARE_DETECT_FAIL,		//�豸Ӳ�����ʧ��
};

//����Э��������Ϣ
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

	s32        m_dwComport;		           //���ں�
	s32        m_dwAddress;                //���ڵ�ַ(use in com485)
	EMBaudrate m_emBaudrate;	           //������
	EMDataBits m_emDatabits;	           //����λ
	EMParity   m_emParity;		           //У��λ
	EMStopBits m_emStopbits;	           //ֹͣλ
}TICBCOMCfg, *PTICBCOMCfg;
#ifndef WIN32
__attribute__ ((packed))
#endif

//�豸��IP�ṹ����
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
/*�ӿڵ���˳��                                                             */
/*NewConnectObject(tcpserver��udp�����ж϶�Ӧ���ض˿ڵ������Ƿ��Ѿ�������
�ѽ����򲻵��øýӿڣ�Ѱ��֮ǰ�����Ķ���)
/*SetLocalCfg
/*GetLocalCfg����SetLocalCfg֮��UnbindDeviceInfo֮ǰ����ѡ���Ե��ã�
/*BindDeviceInfo
/*IsEqualDeviceInfo����BindDeviceInfo֮���UnbindDeviceInfo֮ǰ����ѡ���Ե��ã�
/*SetConnectStateCB�������ڿ���ͨ��ǰ���ûص���
/*SetTranslateCB�������ڿ���ͨ��ǰ���ûص���
/*StartConnect
/*SendData
/*UnbindDeviceInfo
/*StopConnect
/*DelConnectObject��tcpserver��udp����ȷ��Ŀǰ��ʹ��ͬһ�˿ڵ�������ɾ����
/***************************************************************************/
class IConnectBase
{
//���ⲿ����
public:
	IConnectBase(){};
	virtual ~IConnectBase() {};

public:
	/************************************************************************/
	/* ��ȡ������Ϣ�ӿ�                                                     */
	/************************************************************************/
    virtual const s8 * GetLastErrStr() const = 0;
	virtual u32		GetLastErrNo() const = 0;

	/************************************************************************/
	/* ���ñ������ã�                                                       */
	/* udp����: ptLocalCfgΪTICBAddress����ָ�룬IP�Ͷ˿ھ�Ҫ��д           */
	/*          dwLocalCfgSize ΪTICBAddress����Ĵ�С                      */
	/* tcp���ӣ�ptLocalCfgΪĬ��ֵNULL��dwLocalCfgSizeΪĬ��ֵ0             */
	/* com���ӣ�ptLocalCfg ��дTICBCOMCfg����ָ�룬                         */ 
	/*          dwLocalCfgSize ΪTICBCOMCfg����Ĵ�С                       */
	/* tcp server��ptLocalCfgΪTICBAddress����ָ�룬����д�˿ڼ���          */
	/*             dwLocalCfgSize ΪTICBAddress����Ĵ�С                   */
	/* ����ֵ�ο� EMICBErrcode                                              */
	/************************************************************************/
	virtual u32 SetLocalCfg(const void* ptLocalCfg= NULL, u32 dwLocalCfgSize=0) = 0;

	/************************************************************************/
	/* ��ȡ�������ã�                                                       */
	/* udp����: ptLocalCfgΪTICBAddress����ָ�룬������ϢIP�Ͷ˿ھ���Ч     */
	/*          dwLocalCfgSize ΪTICBAddress����Ĵ�С                      */
	/* tcp���ӣ��ýӿ���Ч                                                  */
	/* com���ӣ�ptLocalCfg ��дTICBCOMCfg����ָ�룬                         */ 
	/*          dwLocalCfgSize ΪTICBCOMCfg����Ĵ�С                       */
	/* tcp server��ptLocalCfgΪTICBAddress����ָ�룬������Ϣ���˿���Ч      */
	/*             dwLocalCfgSize ΪTICBAddress����Ĵ�С                   */
	/* ����ֵ�ο� EMICBErrcode                                              */
	/************************************************************************/
	virtual u32 GetLocalCfg(void *ptLocalCfg=NULL) = 0;

	/************************************************************************/
	/* �������豸����Ϣ��                                                 */
	/* dwObjMark:Ϊʹ�ø����Ӷ���ı�ʶ�����Ը���ʵ��ʹ��ʱ���д��ݣ������� */
	/* �ص������ݻص��л�Я������Ϣ                                         */
	/* udp����: ptDevAddr��Ҫ��д�豸�����õ�IP�Ͷ˿�                       */
	/* tcp���ӣ�ptDevAddr��Ҫ��д�豸�����õ�IP�Ͷ˿�                       */
	/* com���ӣ�ptDevAddr ��ֵ��������Ĭ��ֵΪ��                            */
	/* tcp server��ptDevAddr �н���Ҫ��д�Զ˵�IP����                       */
	/* ����ֵ�ο� EMICBErrcode                                              */
	/************************************************************************/
	virtual u32 BindDeviceInfo(u32 dwObjMark,const TICBAddress* ptDevAddr = NULL) = 0;

	/************************************************************************/
	/* ��������豸����Ϣ��                                               */
	/* dwObjMark:Ϊʹ�ø����Ӷ���ı�ʶ�����Ը���ʵ��ʹ��ʱ���д��ݣ������� */
	/* �ص������ݻص��л�Я������Ϣ                                         */
	/* udp����: ptDevAddr��Ҫ��д�豸�����õ�IP�Ͷ˿�                       */
	/* tcp���ӣ�ptDevAddr��Ҫ��д�豸�����õ�IP�Ͷ˿�                       */
	/* com���ӣ�ptDevAddr ��ֵ��������Ĭ��ֵΪ��                            */
	/* tcp server��ptDevAddr �н���Ҫ��д�Զ˵�IP����                       */
	/* ����ֵ�ο� EMICBErrcode                                              */
	/************************************************************************/
	virtual u32 UnbindDeviceInfo(u32 dwObjMark, const TICBAddress* ptDevAddr=NULL ) = 0;

	/************************************************************************/
	/* ��������豸����Ϣ��                                               */
	/* udp����: ptDevAddr��Ҫ��д�豸�����õ�IP�Ͷ˿�                       */
	/* tcp���ӣ�ptDevAddr��Ҫ��д�豸�����õ�IP�Ͷ˿�                       */
	/* com���ӣ��ýӿڵ�����Ч                                              */
	/* tcp server��ptDevAddr �н���Ҫ��д�Զ˵�IP����                       */
	/************************************************************************/
	virtual BOOL32 IsEqualDeviceInfo(const TICBAddress* ptDevAddr)=0;

	/************************************************************************/
	/* ��������                                                             */
	/* ����ֵ�ο� EMICBErrcode                                              
	/************************************************************************/
	virtual u32 StartConnect(void)=0;

	/************************************************************************/
	/* ֹͣ����                                                             */
	/* ����ֵ�ο� EMICBErrcode                                              */
	/************************************************************************/
	virtual u32 StopConnect() = 0;

	/************************************************************************/
	/* �������豸�������ݵĽӿ�(�첽)                                       */
	/* dwObjMark:Ϊʹ�ø����Ӷ���ı�ʶ����BindDeviceInfo�ӿ��д���ֵ��ͬ   */
	/* pszData:����ֵָ��                                                   */
	/* nLength:���ݳ���                                                     */
	/* dwTimeOut:���ݴ��ݵĳ�ʱʱ��                                         */ 
	/* ����ֵ�ο� EMICBErrcode                                              */
	/************************************************************************/
	virtual u32 PostData(u32 dwObjMark, const u8* pszData, s32 nLength, u32 dwTimeOut = 0) = 0;
	/************************************************************************/
	/* �������豸�������ݵĽӿ�(ͬ��)                                       */
	/* dwObjMark:Ϊʹ�ø����Ӷ���ı�ʶ����BindDeviceInfo�ӿ��д���ֵ��ͬ   */
	/* pszData:����ֵָ��                                                   */
	/* nLength:���ݳ���                                                     */
	/* dwTimeOut:���ݴ��ݵĳ�ʱʱ��                                         */ 
	/* ����udp������Ч                                                       */
	/* ����ֵ�ο� EMICBErrcode                                              */
	/************************************************************************/
	virtual u32 SendData(u32 dwObjMark, const u8* pszData, s32 nLength, u32 dwTimeOut = 0) = 0;

	/************************************************************************/
	/* ��������״̬�Ļص�                                                   */
	/* cb:Ϊ״̬�ص�����                                                    */
	/* pContextΪ���ò������ص���������                                     */
	/************************************************************************/
    virtual void SetConnectStateCB(pfICBConnectFunc cb,void *pContext=NULL) = 0;

	/************************************************************************/
	/* ����������Ϣ�Ļص�                                                   */
	/* cb:Ϊ������Ϣ�ص�����                                                */
	/* pContextΪ���ò������ص���������                                     */
	/************************************************************************/
	virtual void SetTranslateCB(pfICBTranslateFunc cb,void *pContext=NULL) = 0;
};

/*=================================================================
������    :  NewConnectObject
��  ��    :  �������Ӷ���
�㷨ʵ��  : 
����˵��  :  EMKDIProtocolType eType����������
����ֵ˵��:
ע  ��    :  
===================================================================*/
IConnectBase * NewConnectObject(EMICBProtocolType eType);

/*=================================================================
������    :  DelConnectObject
��  ��    :  ɾ�����Ӷ���
�㷨ʵ��  : 
����˵��  :  IConnectBase *pConnectBase��NewConnectObject���������Ӷ���
����ֵ˵��:
ע  ��    :  
===================================================================*/
void DelConnectObject(IConnectBase *pConnectBase);

#endif

