#include "mqinterface.h"
#include "mqkafkamanager.h"
#include "mqkafkaserver.h"
#include "mqkafkasender.h"
#include "mqkafkareceiver.h"

/*====================================================================
函数名 ：MQInit
功能 ： 消息队列库初始化
算法实现：
参数说明：
         MQLogCallBack pLogCallBack     日志回调函数
         KD_PTR pUser                   用户数据
返回值说明 ：BOOL32，标识初始化是否成功
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2017/6/16      V0.1         刘春光
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
函数名 ：MQGetErrDesc
功能 ： 获取错误码的描述字符串
算法实现：
参数说明：
         s32 nErrCode     错误码
返回值说明 ：s8* 错误码字符串
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2017/7/4      V0.1         刘春光
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
函数名 ：MQGetServerHandle
功能 ： 通过receiver/sender句柄获取服务器句柄
        函数不会失败
算法实现：
参数说明：
         MQTOPICHANDLE nTopicHandle 对象句柄
返回值说明 ：MQHANDLE 服务器句柄
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2017/7/5      V0.1         刘春光
====================================================================*/
MQ_API MQHANDLE MQGetServerHandle(MQTOPICHANDLE nTopicHandle)
{
    return SERVERHANDLE(nTopicHandle);
}
/*====================================================================
函数名 ：MQConnectServer
功能 ：连接服务器，返回一个针对服务器的句柄
算法实现：
参数说明：
         PTServerInfo ptServerArr   服务器地址，如果是集群，可以传入数组
         u32 nServerAddrNum         传入的服务器地址数组大小
         const s8* szUsername          服务器用户名
         const s8* szPassword          服务器密码
         s32 nTimeout               超时时间 单位ms
返回值说明 ：
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2017/6/16      V0.1         刘春光
====================================================================*/
MQ_API MQHANDLE MQLoginServer(PTMQServerInfo ptServerArr, u32 nServerAddrNum, const s8* pszUsername, const s8* pszPassword, s32 nTimeout)
{
    MQHANDLE nHandle = GetManager()->LoginKafkaServer(ptServerArr, nServerAddrNum, 
            pszUsername, pszPassword, nTimeout);
    return nHandle;
}
/*====================================================================
函数名 ：MQReConnectServer
功能 ：重新连接服务器，用于断链后的重连操作
算法实现：
参数说明：
         MQHANDLE hServerHandle 服务器句柄
         s32 nTimeout           连接服务器的超时时间
返回值说明 ：
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2017/6/16      V0.1         刘春光
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
函数名 ：MQDisConnectServer
功能 ：断开到指定服务器的连接
算法实现：会清除所有相关的receiver和sender
参数说明：
         MQHANDLE hServerHandle 服务器句柄
返回值说明 ：
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2017/6/16      V0.1         刘春光
====================================================================*/
MQ_API s32 MQLogoutServer(MQHANDLE hServerHandle)
{
    return GetManager()->LogoutKafkaServer(hServerHandle);
}
/*====================================================================
函数名 ：MQGetServerState
功能 ：获取目标状态
算法实现：
参数说明：
         MQHANDLE hServerHandle 服务器句柄
返回值说明 ：s32 服务器状态数值
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2017/6/16      V0.1         刘春光
====================================================================*/
MQ_API s32 MQGetServerState(MQHANDLE hServerHandle)
{
    return GetManager()->GetMQState(hServerHandle);
}

/*====================================================================
函数名 ：MQGetAllTopics
功能 ：获取目标所有topic
算法实现：
参数说明：
         MQHANDLE hServerHandle     服务器句柄
         s32 nTimeout               超时时间
         PTMQTopicInfo pTopicArr    [OUT]topic数组
         u32 &nTopicCnt             [OUT]topic的个数
返回值说明 ：
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2017/6/16      V0.1         刘春光
====================================================================*/
MQ_API s32 MQGetAllTopics(MQHANDLE hServerHandle, s32 nTimeout,OUT PTMQTopicInfo pTopicArr,OUT u32 &nTopicCnt)
{
    return GetManager()->GetAllTopics(hServerHandle, nTimeout, pTopicArr, nTopicCnt);
}

/*====================================================================
函数名 ：MQPrintServerData
功能 ：  打印消息队列服务器的详细信息
算法实现：详细信息会在日志回调中被打印出来
参数说明：
         MQHANDLE hServerHandle 要打印的服务器句柄，如果为空，则打印所有的
返回值说明 ：
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2017/6/16      V0.1         刘春光
====================================================================*/
MQ_API void MQPrintServerData(MQHANDLE hServerHandle)
{
    GetManager()->PrintServerData(hServerHandle);
}

// ******************消息接收部分******************
/*
执行顺序:MQInit -> MQConnectServer -> MQCreateReceiver -> MQStartReveive -> MQStopReceive -> MQDeleteReceiver -> MQDisConnectServer
*/
/*====================================================================
函数名 ：MQCreateReceiver
功能 ：创建消息接收器
算法实现：
参数说明：
         MQHANDLE hServerHandle     连接到的服务器句柄
         PTMQTopicInfo pTopicArr    要订阅的topic数组
         u32 nTopicNum              订阅的topic数目
         TMQReceiverParam tReceiverParam    创建receiver的参数 
         s32 nTimeout               超时时间
返回值说明 ：MQTOPICHANDLE 创建出的receiverId
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2017/6/16      V0.1         刘春光
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
函数名 ：MQCreatePreciseReceiver
功能 ：创建精密的消息接收器,此接口不可靠，目前仅对kafka可用
算法实现：所谓“精密”，指的是low-level版本，用户可以自主决定是否提交消息偏移量
    以控制客户端消息偏移位置。
参数说明：
         MQHANDLE hServerHandle     连接到的服务器句柄
         PTMQTopicInfo pTopicArr    要订阅的topic数组
         u32 nTopicNum              订阅的topic数目
         TMQPreciseReceiverParam tReceiverParam    精密receiver的参数，与普通receiver的区别是回调函数不一样 
         s32 nTimeout               超时时间
返回值说明 ：MQTOPICHANDLE 创建出的receiverId
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2019/03/01      V1.1         刘春光                                新增可自主控制的高级接收器接口
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
函数名 ：MQGetReceiverInfo
功能 ：获取指定receiver的详细信息
算法实现：
参数说明：
         MQTOPICHANDLE handle   receiver句柄
         TMQReceiverInfo &tReceiverInfo [OUT]读取到的receiver信息结构体 
返回值说明 ：
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2017/6/16      V0.1         刘春光
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
函数名 ：MQStartReveive
功能 ：开始消息接收
算法实现：数据接收完成（到达结尾）信号会在MQEventCallBack中回调
          注意：服务器断链并手动重连之后，不会自动开始接收数据，还需要手动执行MQStartReveive开始数据的接收
参数说明：
         MQTOPICHANDLE handle   receiver句柄
         s32 nOffset            消息接收起始位置，默认当前，即ENDING，FROM_BEGINNING=-1，其他值为手动指定 
返回值说明 ：
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2017/6/16      V0.1         刘春光
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
函数名 ：MQStopReceive
功能 ：停止接收消息
算法实现：
参数说明：
         MQTOPICHANDLE handle   receiver句柄
返回值说明 ：
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2017/6/16      V0.1         刘春光
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
函数名 ：MQDeleteReceiver
功能 ：销毁消息接收器
算法实现：
参数说明：
         MQTOPICHANDLE handle   receiver句柄
返回值说明 ：
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2017/6/16      V0.1         刘春光
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
函数名 ：MQReceiverCommit
功能 ：提交接收器指向的消息地址,此接口不可靠，目前仅对kafka可用
算法实现：将当前接收器(重)定向到指定的消息位置，可用来标识指定的消息有没有被成功消费
    此操作提交的设置服务器端记录的客户端消息偏移地址，客户端本地也会记录一份偏移地址，
    即便是一直不提交，对于客户端自身也是不会导致消息重复的。
参数说明：
         MQTOPICHANDLE handle   receiver句柄，此命令仅对精密接收器(PreciseReceiver)生效
         const char* topic      消息所属的topic，可从消息回调中获得
         int partition          消息所属的分区号，可从消息回调中获得
         int offset             消息所属的偏移地址，可从消息回调中获得
         BOOL32 async           是否异步提交，异步提交时提交结果可从提交回调中获得,
            原则上异步提交失败不需要重发提交命令
返回值说明 ：
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2019/03/01      V1.1         刘春光                                新增消息偏移提交接口
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
函数名 ：MQReceiverSeek
功能 ：设置提交接收器指向的消息地址，会同时设置客户端和服务器端的偏移量，下次接收数据从此位置开始
    ,此接口不可靠，目前仅对kafka可用
    *\p 会与autoCommit=true冲突!
    *\p 警告：会导致数据重复，需自己处理重复数据!
算法实现：将当前接收器(重)定向到指定的消息位置，可用来标识指定的消息有没有被成功消费
    
参数说明：
         MQTOPICHANDLE handle   receiver句柄，此命令仅对精密接收器(PreciseReceiver)生效
         const char* topic      消息所属的topic，可从消息回调中获得
         int partition          消息所属的分区号，可从消息回调中获得
         int offset             消息所属的偏移地址，可从消息回调中获得
         BOOL32 async           是否异步提交，异步提交时提交结果可从提交回调中获得,
            原则上异步提交失败不需要重发提交命令
返回值说明 ：
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2019/03/08      V1.5         刘春光                                新增消息偏移seek接口
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
// ******************消息接收结束******************

// ******************消息发送部分******************
/*
执行顺序:MQInit -> MQConnectServer -> MQCreateSender -> MQSendMessage -> MQDeleteSender -> MQDisConnectServer
*/
/*====================================================================
函数名 ：MQCreateSender
功能 ：创建消息发送器
算法实现：
参数说明：
         MQHANDLE hServerHandle     连接到的服务器句柄
         const s8* pszTopic             发送消息的目标topic，一个sender只能向一个topic发送数据
         TMQSenderParam tSendParam  创建sender的参数
         s32 nTimeout               超时时间
返回值说明 ：MQTOPICHANDLE          返回创建的sender句柄
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2017/6/16      V0.1         刘春光
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
函数名 ：MQSendMessage
功能 ：发送消息
算法实现：
参数说明：
         MQTOPICHANDLE handle       sender句柄
         const u8* pszValue, s32 nValueLen  发送的数据及数据长度
         const u8* pszKey, s32 nKeyLen      发送数据的key及key长度
         bool bSync = false             是否同步发送，默认为false，异步发送
         bool bType                     消息发送类型，0:单发,1：群发。未使用
         s32 nTimeout                   超时时间
返回值说明 ：
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2017/6/16      V0.1         刘春光
====================================================================*/
MQ_API s32 MQSendMessage(MQTOPICHANDLE handle, const u8* pszValue, s32 nValueLen, const u8* pszKey, 
    s32 nKeyLen , bool bSync , bool bType /*未使用*/, s32 nTimeout)
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
函数名 ：MQDeleteSender
功能 ：销毁发送器
算法实现：
参数说明：
         MQTOPICHANDLE handle       sender句柄
返回值说明 ：
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2017/6/16      V0.1         刘春光
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