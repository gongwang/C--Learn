// test.cpp : 定义控制台应用程序的入口点。
//
#include "kdvtype.h"
#include "mqinterface.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <cstring>
#include <time.h>
#include <sys/timeb.h>
#ifdef _MSC_VER
#include <windows.h> 
#include "wingetopt.h"
#include <process.h>
#include "pthread_w32.h"
#else
#include <unistd.h>
#include <pthread.h>
#include <getopt.h>
#endif
#ifdef _MSC_VER
#define sleep(nseconds) Sleep(nseconds * 1000);
#endif

bool g_bQuitDirect = false;
static int g_nWaitEof = 0;  /* number of partitions awaiting EOF */

void* ReConnectThread(void* args)
{
    MQHANDLE hServer = (MQHANDLE)(long)args;
    if (0 == hServer)
    {
        return NULL;
    }
    sleep(1);
    while(FALSE == MQReConnectServer(hServer, 1000))
    {
        sleep(10);
    }
    printf("reconnect server success\n");
    return 0;
}
void ReConnAfter1s(MQHANDLE hServer)
{
    //TODO check mqkafkaserver has been down or not
    //创建线程用来进行延迟处理，防止主线程阻塞导致后续行为出错
    pthread_t tThread;
    // 创建detached线程 
    pthread_attr_t attr;     
    pthread_attr_init(&attr); 
    pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);    
    int ret = pthread_create(&tThread, &attr, ReConnectThread, (void*)hServer);
    if (ret != 0)
    {
        printf("create ReConnectThread failed: error_code=%d\n", ret );
    }
    pthread_attr_destroy (&attr);
}
/**
 * @brief Signal termination of program
 */
static bool run = true;
static void onstop (int sig) 
{
    run = false;
//        fclose(stdin); /* abort fgets() */
}
int g_nMsgCount = 0;
void msgcb(MQTOPICHANDLE handle, PTMQMessage pMessage, void* pUser)
{
    ++g_nMsgCount;
    printf("\tmessage:Handle=0x%X topic:%s time:%d:%d",
        handle, pMessage->szTopic, pMessage->timestamp.time, pMessage->timestamp.millitm);
    std::string strKey((const char*)pMessage->pszKey, pMessage->nKeyLen);
    printf("key:len=%d,val=%s, value:len=%d,val=%s\n", 
        pMessage->nKeyLen, 0 != pMessage->nKeyLen ? (const u8*)strKey.c_str() : (const u8*)"", pMessage->nValueLen, pMessage->pszValue);
    //printf("recv total msg:%d\n", g_nMsgCount);
}
long g_lLastOffset = 0;
u32 precmsgcb(MQTOPICHANDLE handle, PTMQMessage pMessage, int partition, long offset, void* pUser)
{
    ++g_nMsgCount;
    printf("\tmessage:Handle=0x%X topic:%s time:%d:%d pattition:%d offset:%d ",
        handle, pMessage->szTopic, pMessage->timestamp.time, pMessage->timestamp.millitm, partition, offset);
    std::string strKey((const char*)pMessage->pszKey, pMessage->nKeyLen);
    printf("key:len=%d,val=%s, value:len=%d,val=%s\n", 
        pMessage->nKeyLen, 0 != pMessage->nKeyLen ? (const u8*)strKey.c_str() : (const u8*)"", pMessage->nValueLen, pMessage->pszValue);
    //printf("recv total msg:%d\n", g_nMsgCount);
    if (g_nMsgCount %3 == 1)
    {
        g_lLastOffset = offset;
    }
    if (g_nMsgCount %3 == 2)
    {
//         MQReceiverSeek(handle, pMessage->szTopic, partition, g_lLastOffset, 1);
//     }
//     else
//     {
        MQReceiverCommit(handle, pMessage->szTopic, partition, offset, true);
    }
    return 0;
}
void commitcb(MQTOPICHANDLE handle, const char* topic, int partition, long offset, int errCode, void* pUser)
{
    printf("\tcommitcb:Handle=0x%X pattition:%d offset:%d errcode:%d\n",
        handle,  partition, offset, errCode);
}

void eventcb(MQTOPICHANDLE handle, s32 nErrno, s8* pszErr, s32 nErrLen, void* pUser)
{
    printf("\tEVENT:handle:%X err:%d errstr:%s\n", handle, nErrno, pszErr);
    if (nErrno == MQ_ERR_SERVER_DOWN)
    {
        printf("eventcb server down ,will reconnect after 1s\n");
    }

}
void downcb(MQHANDLE handle, void* pUser)
{
    printf("downcb server down ,will reconnect after 1s\n");
    //ReConnAfter1s(MQGetServerHandle(handle));
    ReConnectThread((void*) handle);
}
void logcb(eMQLoglev eLevel, const s8* pszLog, u32 nLogLen, void* pUser)
{
    //if(MQ_LOG_NORMAL_LEVEL < eLevel)
    printf("LOG:lev=%d %s", eLevel, pszLog);
}
void CheckKey(bool bIsKey)
{
    if (bIsKey){printf("enter message key:\n");}else{printf("enter message value:\n");}
}
int main(int argc, char **argv)
{
    char mode = 'R';
    int opt;
    std::string topic_str;
    std::string brokers;
    std::string group;
    std::string username;
    std::string password;
    std::string clientname;
    bool bsync = false;

    if(!MQInit(logcb, downcb, NULL))
    {
        printf("mqinit failed!\n");
        return 1;
    }
    //分析输入参数
    while ((opt = getopt(argc, argv, "CPLqt:b:g:")) != -1) {
    switch (opt) {
    case 'P':
    case 'C':
    case 'L':
        mode = opt;
        break;
    case 't':
      topic_str = optarg;
      break;
    case 'b':
      brokers = optarg;
      break;
    case 'g':
      {
          group = optarg;
      }
    break;
    case 'q':
        {
            g_bQuitDirect = true;
        }
        break;
    default:
      goto usage;
    }
  }
  if (brokers.empty() || 0 == mode || (topic_str.empty() && 'L' != mode) || optind != argc) {
  usage:
    fprintf(stderr,
            "Usage: %s -C/P/L -b <host1:port1,host2:port2,..> -t <topic>  \n"
            "\n"
            " Options:\n"
            "  -C | -P         Consumer or Producer mode\n"
            "  -L              List the topics info\n"
            "  -b <brokers>    Broker address (localhost:9092)\n"
            "  -t <topic>      Topic to fetch / produce\n"
            "  -g <groupid>    groupid the consumer will belong\n"
            "  -q              consumer only,is quit while all msg received\n"
            "\n"
            "\n",
	    argv[0]);
	exit(1);
  }
  if ('C' != mode)
  {
      g_bQuitDirect = false;
  }
  printf("get param over,topic:%s,brokers:%s\n", topic_str.c_str(), brokers.c_str());
    //拆分并设置服务器地址参数
    std::string::size_type hostpos = 0,portpos = 0,startpos = 0;
    //brokers+=",";//扩展字符串以方便操作
    int size=brokers.length();
    //先统计“，”个数，用于保存服务器地址
    int nServerNum = 0;
    do 
    {
        nServerNum += 1;
        hostpos = brokers.find(',', hostpos + 1);
    } while (hostpos != std::string::npos);
    PTMQServerInfo ptServerInfo = new TMQServerInfo[nServerNum];
    hostpos = 0;
    std::string tmpStr;
    for (int i = 0; i < nServerNum - 1; ++i)
    {
        startpos = 0 == hostpos ? 0 : hostpos + 1;
        portpos = brokers.find(':', startpos);
        hostpos = brokers.find(',', startpos);
        tmpStr = brokers.substr(startpos, portpos - startpos);
        strcpy(ptServerInfo[i].szServerHost, tmpStr.c_str());
        tmpStr = brokers.substr(portpos + 1, hostpos - portpos - 1);
        ptServerInfo[i].nServerPort = atoi(tmpStr.c_str());
//        printf("servrenode %d host:%s port:%d\n", i, ptServerInfo[i].szServerHost, ptServerInfo[i].nServerPort);
    }
    //最后一节
    {
        startpos = 0 == hostpos ? 0 : hostpos + 1;
        portpos = brokers.find(':', startpos);
        hostpos = brokers.length();
        tmpStr = brokers.substr(startpos, portpos - startpos);
        strcpy(ptServerInfo[nServerNum - 1].szServerHost, tmpStr.c_str());
        tmpStr = brokers.substr(portpos + 1, hostpos - portpos - 1);
        ptServerInfo[nServerNum - 1].nServerPort = atoi(tmpStr.c_str());
//         printf("servrenode %d host:%s port:%d\n", nServerNum - 1, ptServerInfo[nServerNum - 1].szServerHost,
//             ptServerInfo[nServerNum - 1].nServerPort);
    }
    printf("login to server\n");
    u32 topicCount = 0;
    MQHANDLE hServer = MQLoginServer(ptServerInfo, nServerNum, NULL, NULL, 1000);
    if (0 == hServer)
    {
        printf("error:login to server %s failed!\n", brokers.c_str());
        return 1;
    }
    printf("login server success,handle=%d\n",hServer);
//     
//     MQGetAllTopics(hServer, 5000, NULL, topicCount);
//     printf("topicCount=%d\n",topicCount);
    MQPrintServerData(NULL);

    signal(SIGINT, onstop);
    if ('C' == mode)
    {   
        //拆分并设置topic参数
        std::string::size_type pos = 0;
        topic_str+=",";//扩展字符串以方便操作
        int nTopicNum = 0;
        while (1) 
        {
            pos = topic_str.find(',', pos + 1);
            if (pos != std::string::npos)
            {
                nTopicNum += 1;
            }
            else break;
        }
        PTMQTopicInfo ptTopicInfo = new TMQTopicInfo[nTopicNum];
        pos = 0;
        for (int j = 0; j < nTopicNum; ++j)
        {
            startpos = 0 == pos ? 0 : pos + 1;
            pos = topic_str.find(',', startpos);
            tmpStr = topic_str.substr(startpos, pos - startpos);
            strcpy(ptTopicInfo[j].szTopicName, tmpStr.c_str());
        }
//         TMQReceiverParam tReceiverParam;
//         tReceiverParam.nMsgTimeout = 1000;
//         tReceiverParam.pEventCb = eventcb;
//         tReceiverParam.pMessageCb = msgcb;
//         strcpy(tReceiverParam.szGroup, group.c_str());
//         //创建receiver
//         MQTOPICHANDLE hReceiver = MQCreateReceiver(hServer, ptTopicInfo, nTopicNum, tReceiverParam, 5000);
//         if (0 == hReceiver)
//         {
//             printf("ERROR:create receiver failed!\n");
//         }
        TMQPreciseReceiverParam tReceiverParam;
        tReceiverParam.nMsgTimeout = 1000;
        tReceiverParam.pEventCb = eventcb;
        tReceiverParam.pMessageCb = precmsgcb;
        tReceiverParam.bAutoCommit = false;
        tReceiverParam.pCommitCb = commitcb;
        strcpy(tReceiverParam.szGroup, group.c_str());
        MQTOPICHANDLE hReceiver = MQCreatePreciseReceiver(hServer, ptTopicInfo, nTopicNum, tReceiverParam, 5000);
        if (0 == hReceiver)
        {
            printf("ERROR:create receiver failed!\n");
        }
        MQStartReveive(hReceiver);
        //开始接收数据
        //获取并打印receiver信息
        TMQReceiverInfo tInfo;
        MQGetReceiverInfo(hReceiver, tInfo);
        printf("receiver1 clientname:%s,group:%s, serverhandle:%d, topicnum:%d :\n", 
            tInfo.szName, tInfo.szGroup, tInfo.hServer, tInfo.dwTopicInfoArrNum);
        for (int j = 0; j < tInfo.dwTopicInfoArrNum; ++j)
        {
            printf("\ttopic %d: %s\n", j, tInfo.tTopicInfoArr[j].szTopicName);
        }
        printf("recv started!\n");
        //         //停止接收数据
        //         MQStopReceive(hReceiver);
        //         MQDeleteReceiver(hReceiver);
        //         MQLogoutServer(hServer);
        while (run)
        {
            sleep(1);
        }
    }
    else if ('P' == mode)
    {
        //sender
        TMQSenderParam tSenderParam;
        tSenderParam.pEventCb = eventcb;
        //strcpy(tSenderParam.szName, clientname.c_str());
        MQTOPICHANDLE hSender = MQCreateSender(hServer, topic_str.c_str(),tSenderParam, 5000);
        if (0 == hSender)
        {
            printf("create sender for topic:%s failed!\n", topic_str.c_str());
        }
        //控制台获取要发送的数据
        //Read messages from stdin and produce to broker.
        bool bIsKey = true;
        std::string strKey;
        printf("enter message key:\n");
        for (std::string line; run && std::getline(std::cin, line); CheckKey(bIsKey)
            ) 
        {
            if (bIsKey)
            {
                strKey = line;
                bIsKey = false;
                continue;
            }
            if (line.empty()) {
                continue;
            }
            bIsKey = true;
            const u8* pszkey = NULL;
            const u8* pszval = NULL;
            s32 nKeyLen = 0;
            s32 nValLen = 0;
            if (!strKey.empty())
            {
                pszkey = (const u8 *)strKey.c_str();
                nKeyLen = strKey.length() + 1;
            }
            pszval = (const u8 *)line.c_str();
            nValLen = line.length() + 1;
            //send message
            int nRet = MQSendMessage(hSender, pszval, nValLen, pszkey, nKeyLen, bsync,0,1000);
            if (0 != nRet)
            {
                printf("send message failed!err:%d,errstr:%s, key:%s, val:%s\n", nRet, MQGetErrDesc(nRet), pszkey, pszval);
            }
        }
    }
    else
    {

    }
    //if ('R' == mode)
        //receiver

    //else if ('S' == mode)
        
        MQLogoutServer(hServer);
    printf("program exit!\n");
	return 0;
}

