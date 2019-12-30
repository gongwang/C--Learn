#include "mqinterface.h"
#include "mqkafkamanager.h"
#include "mqkafkaserver.h"
#include "mqkafkasender.h"
#include "mqkafkareceiver.h"

/*====================================================================
������ ��MQInit
���� �� ��Ϣ���п��ʼ��
�㷨ʵ�֣�
����˵����
         MQLogCallBack pLogCallBack     ��־�ص�����
         KD_PTR pUser                   �û�����
����ֵ˵�� ��BOOL32����ʶ��ʼ���Ƿ�ɹ�
---------------------------------------------------------------------
�޸ļ�¼��
�� ��           �汾         �޸���             �߶���             �޸ļ�¼
2017/6/16      V0.1         ������
====================================================================*/
MQ_API BOOL32 MQInit(MQLogCallBack pLogCallBack, MQServerDownCallBack pServerDownCallBack, void* pUser)
{
    if (NULL == GetManager())
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "MQInit failed,can't get manager instanse\n");
        return FALSE;
    }
    int nRet = GetManager()->Init(pLogCallBack, pServerDownCallBack, pUser);
    if(MQ_ERR_NO == nRet)
    {
        return TRUE;
    }
    GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "MQInit failed errcode:%d,desc:%s\n", nRet, MQGetErrDesc(nRet));
    return FALSE;
}

/*====================================================================
������ ��MQGetErrDesc
���� �� ��ȡ������������ַ���
�㷨ʵ�֣�
����˵����
         s32 nErrCode     ������
����ֵ˵�� ��s8* �������ַ���
---------------------------------------------------------------------
�޸ļ�¼��
�� ��           �汾         �޸���             �߶���             �޸ļ�¼
2017/7/4      V0.1         ������
====================================================================*/
MQ_API s8* MQGetErrDesc(s32 nErrCode)
{
    switch (nErrCode)
    {
    case MQ_ERR_NO:
        return "no err";
    case MQ_ERR_UNKNOWN:
        return "unknown err";
    case MQ_ERR_SDK:
        return "sdk err";
    case MQ_ERR_HANDLE_NOTFOUND:
        return "handle not found";
    case MQ_ERR_HANDLE_ALREADY_EXISTS:
        return "handle already exists";
    case MQ_ERR_INVALID_PARAM:
        return "invalid param";
    case MQ_ERR_WRONG_LOGIC:
        return "wrong logic";
    case MQ_ERR_TIMEOUT:
        return "timeout";
    case MQ_ERR_SERVER_DOWN:
        return "server down";
    case MQ_ERR_RECEIVER_OUTDATA:
        return "receiver data timeout";
    case MQ_ERR_RECEIVER_MSG_EOF:
        return "receiver got EOF";
    case MQ_ERR_RECEIVER_MSG_EXCEPTION:
        return "precise message return error,reseek offset";
    default:
        return "unknown error code";
    }
}
/*====================================================================
������ ��MQGetServerHandle
���� �� ͨ��receiver/sender�����ȡ���������
        ��������ʧ��
�㷨ʵ�֣�
����˵����
         MQTOPICHANDLE nTopicHandle ������
����ֵ˵�� ��MQHANDLE ���������
---------------------------------------------------------------------
�޸ļ�¼��
�� ��           �汾         �޸���             �߶���             �޸ļ�¼
2017/7/5      V0.1         ������
====================================================================*/
MQ_API MQHANDLE MQGetServerHandle(MQTOPICHANDLE nTopicHandle)
{
    return SERVERHANDLE(nTopicHandle);
}
/*====================================================================
������ ��MQConnectServer
���� �����ӷ�����������һ����Է������ľ��
�㷨ʵ�֣�
����˵����
         PTServerInfo ptServerArr   ��������ַ������Ǽ�Ⱥ�����Դ�������
         u32 nServerAddrNum         ����ķ�������ַ�����С
         const s8* szUsername          �������û���
         const s8* szPassword          ����������
         s32 nTimeout               ��ʱʱ�� ��λms
����ֵ˵�� ��
---------------------------------------------------------------------
�޸ļ�¼��
�� ��           �汾         �޸���             �߶���             �޸ļ�¼
2017/6/16      V0.1         ������
====================================================================*/
MQ_API MQHANDLE MQLoginServer(PTMQServerInfo ptServerArr, u32 nServerAddrNum, const s8* pszUsername, const s8* pszPassword, s32 nTimeout)
{
    MQHANDLE nHandle = GetManager()->LoginKafkaServer(ptServerArr, nServerAddrNum, 
            pszUsername, pszPassword, nTimeout);
    return nHandle;
}
/*====================================================================
������ ��MQReConnectServer
���� ���������ӷ����������ڶ��������������
�㷨ʵ�֣�
����˵����
         MQHANDLE hServerHandle ���������
         s32 nTimeout           ���ӷ������ĳ�ʱʱ��
����ֵ˵�� ��
---------------------------------------------------------------------
�޸ļ�¼��
�� ��           �汾         �޸���             �߶���             �޸ļ�¼
2017/6/16      V0.1         ������
====================================================================*/
MQ_API BOOL32 MQReConnectServer(MQHANDLE hServerHandle, s32 nTimeout)
{
    SmartPtr<CMqKafkaServer> pcServer = GetManager()->FindServer(hServerHandle);
    if (NULL == pcServer)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "MQReConnectServer failed errcode:%d,desc:%s\n", 
            MQ_ERR_HANDLE_NOTFOUND, MQGetErrDesc(MQ_ERR_HANDLE_NOTFOUND));
        return MQ_ERR_HANDLE_NOTFOUND;
    }
    return pcServer->ReConnect(nTimeout);
}

/*====================================================================
������ ��MQDisConnectServer
���� ���Ͽ���ָ��������������
�㷨ʵ�֣������������ص�receiver��sender
����˵����
         MQHANDLE hServerHandle ���������
����ֵ˵�� ��
---------------------------------------------------------------------
�޸ļ�¼��
�� ��           �汾         �޸���             �߶���             �޸ļ�¼
2017/6/16      V0.1         ������
====================================================================*/
MQ_API s32 MQLogoutServer(MQHANDLE hServerHandle)
{
    return GetManager()->LogoutKafkaServer(hServerHandle);
}
/*====================================================================
������ ��MQGetServerState
���� ����ȡĿ��״̬
�㷨ʵ�֣�
����˵����
         MQHANDLE hServerHandle ���������
����ֵ˵�� ��s32 ������״̬��ֵ
---------------------------------------------------------------------
�޸ļ�¼��
�� ��           �汾         �޸���             �߶���             �޸ļ�¼
2017/6/16      V0.1         ������
====================================================================*/
MQ_API s32 MQGetServerState(MQHANDLE hServerHandle)
{
    return GetManager()->GetMQState(hServerHandle);
}

/*====================================================================
������ ��MQGetAllTopics
���� ����ȡĿ������topic
�㷨ʵ�֣�
����˵����
         MQHANDLE hServerHandle     ���������
         s32 nTimeout               ��ʱʱ��
         PTMQTopicInfo pTopicArr    [OUT]topic����
         u32 &nTopicCnt             [OUT]topic�ĸ���
����ֵ˵�� ��
---------------------------------------------------------------------
�޸ļ�¼��
�� ��           �汾         �޸���             �߶���             �޸ļ�¼
2017/6/16      V0.1         ������
====================================================================*/
MQ_API s32 MQGetAllTopics(MQHANDLE hServerHandle, s32 nTimeout,OUT PTMQTopicInfo pTopicArr,OUT u32 &nTopicCnt)
{
    return GetManager()->GetAllTopics(hServerHandle, nTimeout, pTopicArr, nTopicCnt);
}

/*====================================================================
������ ��MQPrintServerData
���� ��  ��ӡ��Ϣ���з���������ϸ��Ϣ
�㷨ʵ�֣���ϸ��Ϣ������־�ص��б���ӡ����
����˵����
         MQHANDLE hServerHandle Ҫ��ӡ�ķ�������������Ϊ�գ����ӡ���е�
����ֵ˵�� ��
---------------------------------------------------------------------
�޸ļ�¼��
�� ��           �汾         �޸���             �߶���             �޸ļ�¼
2017/6/16      V0.1         ������
====================================================================*/
MQ_API void MQPrintServerData(MQHANDLE hServerHandle)
{
    GetManager()->PrintServerData(hServerHandle);
}

// ******************��Ϣ���ղ���******************
/*
ִ��˳��:MQInit -> MQConnectServer -> MQCreateReceiver -> MQStartReveive -> MQStopReceive -> MQDeleteReceiver -> MQDisConnectServer
*/
/*====================================================================
������ ��MQCreateReceiver
���� ��������Ϣ������
�㷨ʵ�֣�
����˵����
         MQHANDLE hServerHandle     ���ӵ��ķ��������
         PTMQTopicInfo pTopicArr    Ҫ���ĵ�topic����
         u32 nTopicNum              ���ĵ�topic��Ŀ
         TMQReceiverParam tReceiverParam    ����receiver�Ĳ��� 
         s32 nTimeout               ��ʱʱ��
����ֵ˵�� ��MQTOPICHANDLE ��������receiverId
---------------------------------------------------------------------
�޸ļ�¼��
�� ��           �汾         �޸���             �߶���             �޸ļ�¼
2017/6/16      V0.1         ������
====================================================================*/
MQ_API MQTOPICHANDLE MQCreateReceiver(MQHANDLE hServerHandle, PTMQTopicInfo pTopicArr, u32 nTopicNum, TMQReceiverParam &tReceiverParam, s32 nTimeout)
{
    SmartPtr<CMqKafkaServer> pcServer = GetManager()->FindServer(hServerHandle);
    if (NULL == pcServer)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "MQCreateReceiver failed errcode:%d,desc:%s\n", 
            MQ_ERR_HANDLE_NOTFOUND, MQGetErrDesc(MQ_ERR_HANDLE_NOTFOUND));
        return MQ_ERR_HANDLE_NOTFOUND;
    }
    return pcServer->CreateReceiver(pTopicArr, nTopicNum, tReceiverParam, nTimeout);
}

/*====================================================================
������ ��MQCreatePreciseReceiver
���� ���������ܵ���Ϣ������,�˽ӿڲ��ɿ���Ŀǰ����kafka����
�㷨ʵ�֣���ν�����ܡ���ָ����low-level�汾���û��������������Ƿ��ύ��Ϣƫ����
    �Կ��ƿͻ�����Ϣƫ��λ�á�
����˵����
         MQHANDLE hServerHandle     ���ӵ��ķ��������
         PTMQTopicInfo pTopicArr    Ҫ���ĵ�topic����
         u32 nTopicNum              ���ĵ�topic��Ŀ
         TMQPreciseReceiverParam tReceiverParam    ����receiver�Ĳ���������ͨreceiver�������ǻص�������һ�� 
         s32 nTimeout               ��ʱʱ��
����ֵ˵�� ��MQTOPICHANDLE ��������receiverId
---------------------------------------------------------------------
�޸ļ�¼��
�� ��           �汾         �޸���             �߶���             �޸ļ�¼
2019/03/01      V1.1         ������                                �������������Ƶĸ߼��������ӿ�
====================================================================*/
MQ_API MQTOPICHANDLE MQCreatePreciseReceiver(MQHANDLE hServerHandle, PTMQTopicInfo pTopicArr, u32 nTopicNum, TMQPreciseReceiverParam &tReceiverParam, s32 nTimeout)
{
    SmartPtr<CMqKafkaServer> pcServer = GetManager()->FindServer(hServerHandle);
    if (NULL == pcServer)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "MQCreateReceiver failed errcode:%d,desc:%s\n", 
            MQ_ERR_HANDLE_NOTFOUND, MQGetErrDesc(MQ_ERR_HANDLE_NOTFOUND));
        return MQ_ERR_HANDLE_NOTFOUND;
    }
    return pcServer->CreatePreciseReceiver(pTopicArr, nTopicNum, tReceiverParam, nTimeout);
}
/*====================================================================
������ ��MQGetReceiverInfo
���� ����ȡָ��receiver����ϸ��Ϣ
�㷨ʵ�֣�
����˵����
         MQTOPICHANDLE handle   receiver���
         TMQReceiverInfo &tReceiverInfo [OUT]��ȡ����receiver��Ϣ�ṹ�� 
����ֵ˵�� ��
---------------------------------------------------------------------
�޸ļ�¼��
�� ��           �汾         �޸���             �߶���             �޸ļ�¼
2017/6/16      V0.1         ������
====================================================================*/
MQ_API s32 MQGetReceiverInfo(MQTOPICHANDLE handle, OUT TMQReceiverInfo &tReceiverInfo)
{
    SmartPtr<CMqKafkaServer> pcServer = GetManager()->FindServer(SERVERHANDLE(handle));
    if (NULL == pcServer)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "MQGetReceiverInfo failed errcode:%d,desc:%s\n", 
            MQ_ERR_HANDLE_NOTFOUND, MQGetErrDesc(MQ_ERR_HANDLE_NOTFOUND));
        return MQ_ERR_HANDLE_NOTFOUND;
    }
    SmartPtr<CMqKafkaReceiver> pcReceiver = pcServer->FindReceiver(handle);
    if (NULL == pcReceiver)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "MQGetReceiverInfo failed errcode:%d,desc:%s\n", 
            MQ_ERR_HANDLE_NOTFOUND, MQGetErrDesc(MQ_ERR_HANDLE_NOTFOUND));
        return MQ_ERR_HANDLE_NOTFOUND;
    }
    return pcReceiver->GetReceiverInfo(tReceiverInfo);
}
/*====================================================================
������ ��MQStartReveive
���� ����ʼ��Ϣ����
�㷨ʵ�֣����ݽ�����ɣ������β���źŻ���MQEventCallBack�лص�
          ע�⣺�������������ֶ�����֮�󣬲����Զ���ʼ�������ݣ�����Ҫ�ֶ�ִ��MQStartReveive��ʼ���ݵĽ���
����˵����
         MQTOPICHANDLE handle   receiver���
         s32 nOffset            ��Ϣ������ʼλ�ã�Ĭ�ϵ�ǰ����ENDING��FROM_BEGINNING=-1������ֵΪ�ֶ�ָ�� 
����ֵ˵�� ��
---------------------------------------------------------------------
�޸ļ�¼��
�� ��           �汾         �޸���             �߶���             �޸ļ�¼
2017/6/16      V0.1         ������
====================================================================*/
MQ_API s32 MQStartReveive(MQTOPICHANDLE handle, s32 nOffset)
{
    SmartPtr<CMqKafkaServer> pcServer = GetManager()->FindServer(SERVERHANDLE(handle));
    if (NULL == pcServer)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "MQStartReveive failed errcode:%d,desc:%s\n", 
            MQ_ERR_HANDLE_NOTFOUND, MQGetErrDesc(MQ_ERR_HANDLE_NOTFOUND));
        return MQ_ERR_HANDLE_NOTFOUND;
    }
    SmartPtr<CMqKafkaReceiver> pcReceiver = pcServer->FindReceiver(handle);
    if (NULL == pcReceiver)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "MQStartReveive failed errcode:%d,desc:%s\n", 
            MQ_ERR_HANDLE_NOTFOUND, MQGetErrDesc(MQ_ERR_HANDLE_NOTFOUND));
        return MQ_ERR_HANDLE_NOTFOUND;
    }
    return pcReceiver->StartReveive(nOffset);
}

/*====================================================================
������ ��MQStopReceive
���� ��ֹͣ������Ϣ
�㷨ʵ�֣�
����˵����
         MQTOPICHANDLE handle   receiver���
����ֵ˵�� ��
---------------------------------------------------------------------
�޸ļ�¼��
�� ��           �汾         �޸���             �߶���             �޸ļ�¼
2017/6/16      V0.1         ������
====================================================================*/
MQ_API s32 MQStopReceive(MQTOPICHANDLE handle)
{
    SmartPtr<CMqKafkaServer> pcServer = GetManager()->FindServer(SERVERHANDLE(handle));
    if (NULL == pcServer)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "MQStopReceive failed errcode:%d,desc:%s\n", 
            MQ_ERR_HANDLE_NOTFOUND, MQGetErrDesc(MQ_ERR_HANDLE_NOTFOUND));
        return MQ_ERR_HANDLE_NOTFOUND;
    }
    SmartPtr<CMqKafkaReceiver> pcReceiver = pcServer->FindReceiver(handle);
    if (NULL == pcReceiver)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "MQStopReceive failed errcode:%d,desc:%s\n", 
            MQ_ERR_HANDLE_NOTFOUND, MQGetErrDesc(MQ_ERR_HANDLE_NOTFOUND));
        return MQ_ERR_HANDLE_NOTFOUND;
    }
    return pcReceiver->StopReceive();
}

/*====================================================================
������ ��MQDeleteReceiver
���� ��������Ϣ������
�㷨ʵ�֣�
����˵����
         MQTOPICHANDLE handle   receiver���
����ֵ˵�� ��
---------------------------------------------------------------------
�޸ļ�¼��
�� ��           �汾         �޸���             �߶���             �޸ļ�¼
2017/6/16      V0.1         ������
====================================================================*/
MQ_API s32 MQDeleteReceiver(MQTOPICHANDLE handle)
{
    SmartPtr<CMqKafkaServer> pcServer = GetManager()->FindServer(SERVERHANDLE(handle));
    if (NULL == pcServer)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "MQDeleteReceiver failed errcode:%d,desc:%s\n", 
            MQ_ERR_HANDLE_NOTFOUND, MQGetErrDesc(MQ_ERR_HANDLE_NOTFOUND));
        return MQ_ERR_HANDLE_NOTFOUND;
    }
    return pcServer->DeleteReceiver(handle);
}

/*====================================================================
������ ��MQReceiverCommit
���� ���ύ������ָ�����Ϣ��ַ,�˽ӿڲ��ɿ���Ŀǰ����kafka����
�㷨ʵ�֣�����ǰ������(��)����ָ������Ϣλ�ã���������ʶָ������Ϣ��û�б��ɹ�����
    �˲����ύ�����÷������˼�¼�Ŀͻ�����Ϣƫ�Ƶ�ַ���ͻ��˱���Ҳ���¼һ��ƫ�Ƶ�ַ��
    ������һֱ���ύ�����ڿͻ�������Ҳ�ǲ��ᵼ����Ϣ�ظ��ġ�
����˵����
         MQTOPICHANDLE handle   receiver�������������Ծ��ܽ�����(PreciseReceiver)��Ч
         const char* topic      ��Ϣ������topic���ɴ���Ϣ�ص��л��
         int partition          ��Ϣ�����ķ����ţ��ɴ���Ϣ�ص��л��
         int offset             ��Ϣ������ƫ�Ƶ�ַ���ɴ���Ϣ�ص��л��
         BOOL32 async           �Ƿ��첽�ύ���첽�ύʱ�ύ����ɴ��ύ�ص��л��,
            ԭ�����첽�ύʧ�ܲ���Ҫ�ط��ύ����
����ֵ˵�� ��
---------------------------------------------------------------------
�޸ļ�¼��
�� ��           �汾         �޸���             �߶���             �޸ļ�¼
2019/03/01      V1.1         ������                                ������Ϣƫ���ύ�ӿ�
====================================================================*/
MQ_API s32 MQReceiverCommit(MQTOPICHANDLE handle, const char* topic, int partition, long offset, BOOL32 async)
{
    SmartPtr<CMqKafkaServer> pcServer = GetManager()->FindServer(SERVERHANDLE(handle));
    if (NULL == pcServer)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "MQStopReceive failed errcode:%d,desc:%s\n", 
            MQ_ERR_HANDLE_NOTFOUND, MQGetErrDesc(MQ_ERR_HANDLE_NOTFOUND));
        return MQ_ERR_HANDLE_NOTFOUND;
    }
    SmartPtr<CMqKafkaReceiver> pcReceiver = pcServer->FindReceiver(handle);
    if (NULL == pcReceiver)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "MQStopReceive failed errcode:%d,desc:%s\n", 
            MQ_ERR_HANDLE_NOTFOUND, MQGetErrDesc(MQ_ERR_HANDLE_NOTFOUND));
        return MQ_ERR_HANDLE_NOTFOUND;
    }
    return pcReceiver->Commit(topic, partition, offset, async);
}
/*====================================================================
������ ��MQReceiverSeek
���� �������ύ������ָ�����Ϣ��ַ����ͬʱ���ÿͻ��˺ͷ������˵�ƫ�������´ν������ݴӴ�λ�ÿ�ʼ
    ,�˽ӿڲ��ɿ���Ŀǰ����kafka����
    *\p ����autoCommit=true��ͻ!
    *\p ���棺�ᵼ�������ظ������Լ������ظ�����!
�㷨ʵ�֣�����ǰ������(��)����ָ������Ϣλ�ã���������ʶָ������Ϣ��û�б��ɹ�����
    
����˵����
         MQTOPICHANDLE handle   receiver�������������Ծ��ܽ�����(PreciseReceiver)��Ч
         const char* topic      ��Ϣ������topic���ɴ���Ϣ�ص��л��
         int partition          ��Ϣ�����ķ����ţ��ɴ���Ϣ�ص��л��
         int offset             ��Ϣ������ƫ�Ƶ�ַ���ɴ���Ϣ�ص��л��
         BOOL32 async           �Ƿ��첽�ύ���첽�ύʱ�ύ����ɴ��ύ�ص��л��,
            ԭ�����첽�ύʧ�ܲ���Ҫ�ط��ύ����
����ֵ˵�� ��
---------------------------------------------------------------------
�޸ļ�¼��
�� ��           �汾         �޸���             �߶���             �޸ļ�¼
2019/03/08      V1.5         ������                                ������Ϣƫ��seek�ӿ�
====================================================================*/
MQ_API s32 MQReceiverSeek(MQTOPICHANDLE handle, const char* topic, int partition, long offset, s32 nTimeout)
{
    SmartPtr<CMqKafkaServer> pcServer = GetManager()->FindServer(SERVERHANDLE(handle));
    if (NULL == pcServer)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "MQStopReceive failed errcode:%d,desc:%s\n", 
            MQ_ERR_HANDLE_NOTFOUND, MQGetErrDesc(MQ_ERR_HANDLE_NOTFOUND));
        return MQ_ERR_HANDLE_NOTFOUND;
    }
    SmartPtr<CMqKafkaReceiver> pcReceiver = pcServer->FindReceiver(handle);
    if (NULL == pcReceiver)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "MQStopReceive failed errcode:%d,desc:%s\n", 
            MQ_ERR_HANDLE_NOTFOUND, MQGetErrDesc(MQ_ERR_HANDLE_NOTFOUND));
        return MQ_ERR_HANDLE_NOTFOUND;
    }
    return pcReceiver->Seek(topic, partition, offset, nTimeout);
}
// ******************��Ϣ���ս���******************

// ******************��Ϣ���Ͳ���******************
/*
ִ��˳��:MQInit -> MQConnectServer -> MQCreateSender -> MQSendMessage -> MQDeleteSender -> MQDisConnectServer
*/
/*====================================================================
������ ��MQCreateSender
���� ��������Ϣ������
�㷨ʵ�֣�
����˵����
         MQHANDLE hServerHandle     ���ӵ��ķ��������
         const s8* pszTopic             ������Ϣ��Ŀ��topic��һ��senderֻ����һ��topic��������
         TMQSenderParam tSendParam  ����sender�Ĳ���
         s32 nTimeout               ��ʱʱ��
����ֵ˵�� ��MQTOPICHANDLE          ���ش�����sender���
---------------------------------------------------------------------
�޸ļ�¼��
�� ��           �汾         �޸���             �߶���             �޸ļ�¼
2017/6/16      V0.1         ������
====================================================================*/
MQ_API MQTOPICHANDLE MQCreateSender(MQHANDLE hServerHandle, const s8* pszTopic, TMQSenderParam &tSendParam, s32 nTimeout)
{
    SmartPtr<CMqKafkaServer> pcServer = GetManager()->FindServer(hServerHandle);
    if (NULL == pcServer)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "MQCreateSender failed errcode:%d,desc:%s\n", 
            MQ_ERR_HANDLE_NOTFOUND, MQGetErrDesc(MQ_ERR_HANDLE_NOTFOUND));
        return MQ_ERR_HANDLE_NOTFOUND;
    }
    return pcServer->CreateSender(pszTopic, tSendParam, nTimeout);
}

/*====================================================================
������ ��MQSendMessage
���� ��������Ϣ
�㷨ʵ�֣�
����˵����
         MQTOPICHANDLE handle       sender���
         const u8* pszValue, s32 nValueLen  ���͵����ݼ����ݳ���
         const u8* pszKey, s32 nKeyLen      �������ݵ�key��key����
         bool bSync = false             �Ƿ�ͬ�����ͣ�Ĭ��Ϊfalse���첽����
         bool bType                     ��Ϣ�������ͣ�0:����,1��Ⱥ����δʹ��
         s32 nTimeout                   ��ʱʱ��
����ֵ˵�� ��
---------------------------------------------------------------------
�޸ļ�¼��
�� ��           �汾         �޸���             �߶���             �޸ļ�¼
2017/6/16      V0.1         ������
====================================================================*/
MQ_API s32 MQSendMessage(MQTOPICHANDLE handle, const u8* pszValue, s32 nValueLen, const u8* pszKey, 
    s32 nKeyLen , bool bSync , bool bType /*δʹ��*/, s32 nTimeout)
{
    SmartPtr<CMqKafkaServer> pcServer = GetManager()->FindServer(SERVERHANDLE(handle));
    if (NULL == pcServer)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "MQCreateSender failed errcode:%d,desc:%s\n", 
            MQ_ERR_HANDLE_NOTFOUND, MQGetErrDesc(MQ_ERR_HANDLE_NOTFOUND));
        return MQ_ERR_HANDLE_NOTFOUND;
    }
    if (SERVER_ONLINE != pcServer->GetState())
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "MQCreateSender failed errcode:%d,desc:%s\n", 
            MQ_ERR_SERVER_DOWN, MQGetErrDesc(MQ_ERR_SERVER_DOWN));
        return MQ_ERR_SERVER_DOWN;
    }
    SmartPtr<CMqKafkaSender> pcSender = pcServer->FindSender(handle);
    if (NULL == pcSender)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "MQCreateSender failed errcode:%d,desc:%s\n", 
            MQ_ERR_HANDLE_NOTFOUND, MQGetErrDesc(MQ_ERR_HANDLE_NOTFOUND));
        return MQ_ERR_HANDLE_NOTFOUND;
    }
    return pcSender->SendMessageToMQ(pszValue, nValueLen, pszKey, nKeyLen, bSync, bType, nTimeout);
}

/*====================================================================
������ ��MQDeleteSender
���� �����ٷ�����
�㷨ʵ�֣�
����˵����
         MQTOPICHANDLE handle       sender���
����ֵ˵�� ��
---------------------------------------------------------------------
�޸ļ�¼��
�� ��           �汾         �޸���             �߶���             �޸ļ�¼
2017/6/16      V0.1         ������
====================================================================*/
MQ_API s32 MQDeleteSender(MQTOPICHANDLE handle)
{
    SmartPtr<CMqKafkaServer> pcServer = GetManager()->FindServer(SERVERHANDLE(handle));
    if (NULL == pcServer)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "MQCreateSender failed errcode:%d,desc:%s\n", 
            MQ_ERR_HANDLE_NOTFOUND, MQGetErrDesc(MQ_ERR_HANDLE_NOTFOUND));
        return MQ_ERR_HANDLE_NOTFOUND;
    }
    return pcServer->DeleteSender(handle);
}