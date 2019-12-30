/*====================================================================
模块名 ：puserver
文件名 ：puctrlfunc.cpp
相关文件：puctrlfunc.h
实现功能：实现puctrl协议栈的包装类初始化
作者 ：刘春光
版权 ：<Copyright(C) 2003-2018 Suzhou Keda Technology Co., Ltd. All rights reserved.>
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2018/4/20        V1.0         刘春光
====================================================================*/
#include "mqkafkardfunc.h"

p_rd_kafka_version IRdKafka::_version = NULL;
p_rd_kafka_version_str IRdKafka::_version_str = NULL;
p_rd_kafka_brokers_add IRdKafka::_brokers_add = NULL;
p_rd_kafka_conf_destroy IRdKafka::_conf_destroy = NULL;
p_rd_kafka_conf_dup IRdKafka::_conf_dup = NULL;
p_rd_kafka_conf_new IRdKafka::_conf_new = NULL;
p_rd_kafka_conf_set IRdKafka::_conf_set = NULL;
p_rd_kafka_conf_set_error_cb IRdKafka::_conf_set_error_cb = NULL;
p_rd_kafka_conf_set_log_cb IRdKafka::_conf_set_log_cb = NULL;
p_rd_kafka_conf_set_opaque IRdKafka::_conf_set_opaque = NULL;
p_rd_kafka_conf_set_stats_cb IRdKafka::_conf_set_stats_cb = NULL;
p_rd_kafka_conf_set_throttle_cb IRdKafka::_conf_set_throttle_cb = NULL;
p_rd_kafka_destroy IRdKafka::_destroy = NULL;
p_rd_kafka_err2str IRdKafka::_err2str = NULL;
p_rd_kafka_new IRdKafka::_new = NULL;
p_rd_kafka_poll IRdKafka::_poll = NULL;
p_rd_kafka_set_log_level IRdKafka::_set_log_level = NULL;
p_rd_kafka_topic_conf_new IRdKafka::_topic_conf_new = NULL;
p_rd_kafka_topic_conf_destroy IRdKafka::_topic_conf_destroy = NULL;
p_rd_kafka_topic_name IRdKafka::_topic_name = NULL;
p_rd_kafka_assign IRdKafka::_assign = NULL;
p_rd_kafka_conf_get IRdKafka::_conf_get = NULL;
p_rd_kafka_conf_set_consume_cb IRdKafka::_conf_set_consume_cb = NULL;
p_rd_kafka_conf_set_default_topic_conf IRdKafka::_conf_set_default_topic_conf = NULL;
p_rd_kafka_conf_set_dr_cb IRdKafka::_conf_set_dr_cb = NULL;
p_rd_kafka_conf_set_rebalance_cb IRdKafka::_conf_set_rebalance_cb = NULL;
p_rd_kafka_consumer_close IRdKafka::_consumer_close = NULL;
p_rd_kafka_consumer_poll IRdKafka::_consumer_poll = NULL;
p_rd_kafka_last_error IRdKafka::_last_error = NULL;
p_rd_kafka_message_destroy IRdKafka::_message_destroy = NULL;
p_rd_kafka_message_timestamp IRdKafka::_message_timestamp = NULL;
p_rd_kafka_outq_len IRdKafka::_outq_len = NULL;
p_rd_kafka_poll_set_consumer IRdKafka::_poll_set_consumer = NULL;
p_rd_kafka_subscribe IRdKafka::_subscribe = NULL;
p_rd_kafka_topic_conf_set IRdKafka::_topic_conf_set = NULL;
p_rd_kafka_topic_destroy IRdKafka::_topic_destroy = NULL;
p_rd_kafka_topic_new IRdKafka::_topic_new = NULL;
p_rd_kafka_topic_partition_list_add IRdKafka::_topic_partition_list_add = NULL;
p_rd_kafka_topic_partition_list_destroy IRdKafka::_topic_partition_list_destroy = NULL;
p_rd_kafka_topic_partition_list_new IRdKafka::_topic_partition_list_new = NULL;
p_rd_kafka_topic_partition_list_set_offset IRdKafka::_topic_partition_list_set_offset = NULL;
p_rd_kafka_unsubscribe IRdKafka::_unsubscribe = NULL;
p_rd_kafka_produce IRdKafka::_produce = NULL;
p_rd_kafka_metadata_destroy IRdKafka::_metadata_destroy = NULL;
p_rd_kafka_metadata IRdKafka::_metadata = NULL;
p_rd_kafka_conf_set_offset_commit_cb IRdKafka::_conf_set_offset_commit_cb = NULL;
p_rd_kafka_commit IRdKafka::_commit = NULL;
p_rd_kafka_topic_partition_list_add_range IRdKafka::_topic_partition_list_add_range = NULL;
p_rd_kafka_topic_partition_list_del IRdKafka::_topic_partition_list_del = NULL;
p_rd_kafka_topic_partition_list_del_by_idx IRdKafka::_topic_partition_list_del_by_idx = NULL;
p_rd_kafka_topic_partition_list_copy IRdKafka::_topic_partition_list_copy = NULL;
p_rd_kafka_topic_partition_list_find IRdKafka::_topic_partition_list_find = NULL;
p_rd_kafka_seek IRdKafka::_seek = NULL;
u32_ptr IRdKafka::m_pDll = NULL;

BOOL32 IRdKafka::LoadDll()
{
    if (NULL != m_pDll)
    {
        return TRUE;
    }
    //加载协议栈模块
    const s8 *pErrStr = NULL;
#ifdef _LINUX_
    m_pDll = IcpLoadLibrary("rdkafka", pErrStr);
#elif defined WIN32
    m_pDll = IcpLoadLibrary("librdkafka", pErrStr);
#endif
    if (NULL == m_pDll)
    {
        printf("IRdKafka::LoadDll: LoadLibrary failed, Err=%s\n", pErrStr);
        return FALSE;
    }
    //获取导出接口
    _version = (p_rd_kafka_version) IcpGetProcAddress(m_pDll, "rd_kafka_version");
    _version_str = (p_rd_kafka_version_str) IcpGetProcAddress(m_pDll, "rd_kafka_version_str");
    _brokers_add = (p_rd_kafka_brokers_add)IcpGetProcAddress(m_pDll, "rd_kafka_brokers_add");
    _conf_destroy = (p_rd_kafka_conf_destroy)IcpGetProcAddress(m_pDll, "rd_kafka_conf_destroy");
    _conf_dup = (p_rd_kafka_conf_dup)IcpGetProcAddress(m_pDll, "rd_kafka_conf_dup");
    _conf_new = (p_rd_kafka_conf_new)IcpGetProcAddress(m_pDll, "rd_kafka_conf_new");
    _conf_set = (p_rd_kafka_conf_set)IcpGetProcAddress(m_pDll, "rd_kafka_conf_set");
    _conf_set_error_cb = (p_rd_kafka_conf_set_error_cb)IcpGetProcAddress(m_pDll, "rd_kafka_conf_set_error_cb");
    _conf_set_log_cb = (p_rd_kafka_conf_set_log_cb)IcpGetProcAddress(m_pDll, "rd_kafka_conf_set_log_cb");
    _conf_set_opaque = (p_rd_kafka_conf_set_opaque)IcpGetProcAddress(m_pDll, "rd_kafka_conf_set_opaque");
    _conf_set_stats_cb = (p_rd_kafka_conf_set_stats_cb)IcpGetProcAddress(m_pDll, "rd_kafka_conf_set_stats_cb");
    _conf_set_throttle_cb = (p_rd_kafka_conf_set_throttle_cb)IcpGetProcAddress(m_pDll, "rd_kafka_conf_set_throttle_cb");
    _destroy = (p_rd_kafka_destroy)IcpGetProcAddress(m_pDll, "rd_kafka_destroy");
    _err2str = (p_rd_kafka_err2str)IcpGetProcAddress(m_pDll, "rd_kafka_err2str");
    _new = (p_rd_kafka_new)IcpGetProcAddress(m_pDll, "rd_kafka_new");
    _poll = (p_rd_kafka_poll)IcpGetProcAddress(m_pDll, "rd_kafka_poll");
    _set_log_level = (p_rd_kafka_set_log_level)IcpGetProcAddress(m_pDll, "rd_kafka_set_log_level");
    _topic_conf_new = (p_rd_kafka_topic_conf_new)IcpGetProcAddress(m_pDll, "rd_kafka_topic_conf_new");
    _topic_conf_destroy = (p_rd_kafka_topic_conf_destroy)IcpGetProcAddress(m_pDll, "rd_kafka_topic_conf_destroy");
    _topic_name = (p_rd_kafka_topic_name)IcpGetProcAddress(m_pDll, "rd_kafka_topic_name");
    _assign = (p_rd_kafka_assign)IcpGetProcAddress(m_pDll, "rd_kafka_assign");
    _conf_get = (p_rd_kafka_conf_get)IcpGetProcAddress(m_pDll, "rd_kafka_conf_get");
    _conf_set_consume_cb = (p_rd_kafka_conf_set_consume_cb)IcpGetProcAddress(m_pDll, "rd_kafka_conf_set_consume_cb");
    _conf_set_default_topic_conf = (p_rd_kafka_conf_set_default_topic_conf)IcpGetProcAddress(m_pDll, "rd_kafka_conf_set_default_topic_conf");
    _conf_set_dr_cb = (p_rd_kafka_conf_set_dr_cb)IcpGetProcAddress(m_pDll, "rd_kafka_conf_set_dr_cb");
    _conf_set_rebalance_cb = (p_rd_kafka_conf_set_rebalance_cb)IcpGetProcAddress(m_pDll, "rd_kafka_conf_set_rebalance_cb");
    _consumer_close = (p_rd_kafka_consumer_close)IcpGetProcAddress(m_pDll, "rd_kafka_consumer_close");
    _consumer_poll = (p_rd_kafka_consumer_poll)IcpGetProcAddress(m_pDll, "rd_kafka_consumer_poll");
    _last_error = (p_rd_kafka_last_error)IcpGetProcAddress(m_pDll, "rd_kafka_last_error");
    _message_destroy = (p_rd_kafka_message_destroy)IcpGetProcAddress(m_pDll, "rd_kafka_message_destroy");
    _message_timestamp = (p_rd_kafka_message_timestamp)IcpGetProcAddress(m_pDll, "rd_kafka_message_timestamp");
    _outq_len = (p_rd_kafka_outq_len)IcpGetProcAddress(m_pDll, "rd_kafka_outq_len");
    _poll_set_consumer = (p_rd_kafka_poll_set_consumer)IcpGetProcAddress(m_pDll, "rd_kafka_poll_set_consumer");
    _subscribe = (p_rd_kafka_subscribe)IcpGetProcAddress(m_pDll, "rd_kafka_subscribe");
    _topic_conf_set = (p_rd_kafka_topic_conf_set)IcpGetProcAddress(m_pDll, "rd_kafka_topic_conf_set");
    _topic_destroy = (p_rd_kafka_topic_destroy)IcpGetProcAddress(m_pDll, "rd_kafka_topic_destroy");
    _topic_new = (p_rd_kafka_topic_new)IcpGetProcAddress(m_pDll, "rd_kafka_topic_new");
    _topic_partition_list_add = (p_rd_kafka_topic_partition_list_add)IcpGetProcAddress(m_pDll, "rd_kafka_topic_partition_list_add");
    _topic_partition_list_destroy = (p_rd_kafka_topic_partition_list_destroy)IcpGetProcAddress(m_pDll, "rd_kafka_topic_partition_list_destroy");
    _topic_partition_list_new = (p_rd_kafka_topic_partition_list_new)IcpGetProcAddress(m_pDll, "rd_kafka_topic_partition_list_new");
    _topic_partition_list_set_offset = (p_rd_kafka_topic_partition_list_set_offset)IcpGetProcAddress(m_pDll, "rd_kafka_topic_partition_list_set_offset");
    _unsubscribe = (p_rd_kafka_unsubscribe)IcpGetProcAddress(m_pDll, "rd_kafka_unsubscribe");
    _produce = (p_rd_kafka_produce)IcpGetProcAddress(m_pDll, "rd_kafka_produce");
    _metadata_destroy = (p_rd_kafka_metadata_destroy)IcpGetProcAddress(m_pDll, "rd_kafka_metadata_destroy");
    _metadata = (p_rd_kafka_metadata)IcpGetProcAddress(m_pDll, "rd_kafka_metadata");
    _conf_set_offset_commit_cb = (p_rd_kafka_conf_set_offset_commit_cb)IcpGetProcAddress(m_pDll, "rd_kafka_conf_set_offset_commit_cb");
    _commit = (p_rd_kafka_commit)IcpGetProcAddress(m_pDll, "rd_kafka_commit");
    _seek = (p_rd_kafka_seek)IcpGetProcAddress(m_pDll, "rd_kafka_seek");
    _topic_partition_list_add_range = (p_rd_kafka_topic_partition_list_add_range)IcpGetProcAddress(m_pDll, "rd_kafka_topic_partition_list_add_range");
    _topic_partition_list_del = (p_rd_kafka_topic_partition_list_del)IcpGetProcAddress(m_pDll, "rd_kafka_topic_partition_list_del");
    _topic_partition_list_del_by_idx = (p_rd_kafka_topic_partition_list_del_by_idx)IcpGetProcAddress(m_pDll, "rd_kafka_topic_partition_list_del_by_idx");
    _topic_partition_list_copy = (p_rd_kafka_topic_partition_list_copy)IcpGetProcAddress(m_pDll, "rd_kafka_topic_partition_list_copy");
    _topic_partition_list_find = (p_rd_kafka_topic_partition_list_find)IcpGetProcAddress(m_pDll, "rd_kafka_topic_partition_list_find");
    if ( !_version || !_version_str || !_brokers_add || !_conf_destroy || !_conf_dup || !_conf_new || !_conf_set 
        || !_conf_set_error_cb || !_conf_set_log_cb || !_conf_set_opaque || !_conf_set_stats_cb
        || !_conf_set_throttle_cb || !_destroy || !_err2str || !_new
        || ! _poll || !_set_log_level || !_topic_conf_new || !_topic_conf_destroy || !_topic_name
        || ! _assign || !_conf_get || !_conf_set_consume_cb || !_conf_set_default_topic_conf
        || ! _conf_set_dr_cb || !_conf_set_rebalance_cb || !_consumer_close || !_consumer_poll
        || ! _last_error || !_message_destroy || !_message_timestamp || !_outq_len
        || ! _poll_set_consumer || !_subscribe || !_topic_conf_set || !_topic_destroy
        || ! _topic_new || !_topic_partition_list_add || !_topic_partition_list_destroy || !_topic_partition_list_new
        || ! _topic_partition_list_set_offset || !_unsubscribe || !_produce
        || !_metadata_destroy || !_metadata 
        || !_conf_set_offset_commit_cb || !_commit || !_seek || !_topic_partition_list_add_range
        || !_topic_partition_list_del || !_topic_partition_list_del_by_idx || !_topic_partition_list_copy
        || !_topic_partition_list_find
        )
    {
        printf("IRdKafka::LoadDll: Get function addr error!\n");
        IcpFreeLibrary(m_pDll);
        m_pDll = NULL;
        return FALSE;
    }
    return TRUE;
}
void IRdKafka::ReleaseDll()
{
    IcpFreeLibrary(m_pDll);
    m_pDll = NULL;
}
