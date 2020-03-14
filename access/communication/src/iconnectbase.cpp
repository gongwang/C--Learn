#include "iconnectbase.h"
#include "tcpconnectpolicy.h"
#include "tcpserverpolicy.h"
#include "udpconnectpolicy.h"
#include "comconnectpolicy.h"


IConnectBase * NewConnectObject(EMICBProtocolType eType)
{
	switch(eType)
	{
	case EMICB_PROTO_Tcp:
		return new CTcpConnectPolicy();
		break;

	case EMICB_PROTO_Udp:
		return new CUdpConnectPolicy();
		break;

	case EMICB_PROTO_Com:
		return new CComConnectPolicy();
		break;

	case EMICB_PROTO_TCPSERVER:
		printf("new tcp server\n");
		return new CTcpServerPolicy();
		break;
	default:
		return NULL;
		break;
	}
}

void DelConnectObject(IConnectBase* pcConnectBasepolicy)
{

	if (NULL != pcConnectBasepolicy)	
	{
		delete pcConnectBasepolicy;
		pcConnectBasepolicy = NULL;
	}
}
