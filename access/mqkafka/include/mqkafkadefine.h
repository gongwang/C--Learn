#ifndef _MQKAFKADEFINE_H_
#define _MQKAFKADEFINE_H_
#include "mqkafkardfunc.h"
#ifdef max
#undef max
#undef min
#endif
#ifdef _MSC_VER
#include "pthread_w32.h"
#else
#include <unistd.h>
#include <pthread.h>
#endif //_MSC_VER
#include "smartptr.h"

#ifdef _MSC_VER
#define sleep(x) Sleep(x * 1000)
#endif //_MSC_VER

//屏蔽vsnprintf的非安全性警告
#pragma warning(disable:4996)

enum eMqKafkaMsg
{
    MQ_KAFKA_MSG_UNKNOWN = 0,
    MQ_KAFKA_MSG_LOG,
    MQ_KAFKA_MSG_STATS,
    MQ_KAFKA_MSG_THROTTLE,
    MQ_KAFKA_MSG_ERROR,
    MQ_KAFKA_MSG_EVENT,             //mqkafka内部自定义的消息，详见eMQErrCode，用于直接触发消息，不需要转换
};
//mqkafka基础类，用于事件回调统一处理
class CMqKafkaBase
{
public:
    CMqKafkaBase():m_prkServer(NULL), m_prkConf(NULL), m_nConnectTimeout(0){}
    ~CMqKafkaBase()
    {
        if (NULL != m_prkServer)
        {
            IRdKafka::_destroy(m_prkServer);
        }
        if (NULL != m_prkConf)
        {
            IRdKafka::_conf_destroy(m_prkConf);
        }
    }
    //服务器断链事件
    virtual void OnDisConnect() = 0;
    //服务器消息事件处理函数
    virtual void OnMessageCb(eMqKafkaMsg eMsgType, int nErrNo, int nMsgParam, void* pMsgVal, int nMsgLen) = 0;
    rd_kafka_t *m_prkServer;
    rd_kafka_conf_t *m_prkConf;
    unsigned int m_nConnectTimeout;
};
#endif  //_MQKAFKADEFINE_H_