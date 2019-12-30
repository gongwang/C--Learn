#ifndef _MQKAFKARDFUNC_H_
#define _MQKAFKARDFUNC_H_
#include "rdkafka.h"
#include "icp/icp.h"

//rdkafka库的接口符号声明
typedef int (*p_rd_kafka_version)(void);
typedef const char * (*p_rd_kafka_version_str) (void);
typedef int (*p_rd_kafka_brokers_add)(rd_kafka_t *rk, const char *brokerlist);	
typedef void (*p_rd_kafka_conf_destroy)(rd_kafka_conf_t *conf);
typedef rd_kafka_conf_t *(*p_rd_kafka_conf_dup)(const rd_kafka_conf_t *conf);	
typedef rd_kafka_conf_t *(*p_rd_kafka_conf_new)(void);
typedef rd_kafka_conf_res_t (*p_rd_kafka_conf_set)(rd_kafka_conf_t *conf,
    const char *name,
    const char *value,
    char *errstr, size_t errstr_size);
typedef void (*p_rd_kafka_conf_set_error_cb)(rd_kafka_conf_t *conf,
    void  (*error_cb) (rd_kafka_t *rk, int err,
    const char *reason,
    void *opaque));
typedef void (*p_rd_kafka_conf_set_log_cb)(rd_kafka_conf_t *conf,
    void (*log_cb) (const rd_kafka_t *rk, int level,
    const char *fac, const char *buf));
typedef void (*p_rd_kafka_conf_set_opaque)(rd_kafka_conf_t *conf, void *opaque);
typedef void (*p_rd_kafka_conf_set_stats_cb)(rd_kafka_conf_t *conf,
    int (*stats_cb) (rd_kafka_t *rk,
    char *json,
    size_t json_len,
    void *opaque));
typedef void (*p_rd_kafka_conf_set_throttle_cb)(rd_kafka_conf_t *conf,
    void (*throttle_cb) (
    rd_kafka_t *rk,
    const char *broker_name,
    int32_t broker_id,
    int throttle_time_ms,
    void *opaque));
typedef void (*p_rd_kafka_destroy)(rd_kafka_t *rk);
typedef const char* (*p_rd_kafka_err2str)(rd_kafka_resp_err_t err);
typedef rd_kafka_t* (*p_rd_kafka_new)(rd_kafka_type_t type, rd_kafka_conf_t *conf,
    char *errstr, size_t errstr_size);
typedef int (*p_rd_kafka_poll)(rd_kafka_t *rk, int timeout_ms);
typedef void (*p_rd_kafka_set_log_level)(rd_kafka_t *rk, int level);
typedef rd_kafka_topic_conf_t *(*p_rd_kafka_topic_conf_new)(void);
typedef void (*p_rd_kafka_topic_conf_destroy)(rd_kafka_topic_conf_t *topic_conf);
typedef const char * (*p_rd_kafka_topic_name)(const rd_kafka_topic_t *rkt);
typedef rd_kafka_resp_err_t (*p_rd_kafka_assign)(rd_kafka_t *rk,
    const rd_kafka_topic_partition_list_t *partitions);
typedef rd_kafka_conf_res_t (*p_rd_kafka_conf_get)(const rd_kafka_conf_t *conf,
    const char *name,
    char *dest, size_t *dest_size);
typedef void (*p_rd_kafka_conf_set_consume_cb)(rd_kafka_conf_t *conf,
    void (*consume_cb) (rd_kafka_message_t *
    rkmessage,
    void *opaque));
typedef void (*p_rd_kafka_conf_set_default_topic_conf)(rd_kafka_conf_t *conf,
    rd_kafka_topic_conf_t *tconf);
typedef void (*p_rd_kafka_conf_set_dr_cb)(rd_kafka_conf_t *conf,
    void (*dr_cb) (rd_kafka_t *rk,
    void *payload, size_t len,
    rd_kafka_resp_err_t err,
    void *opaque, void *msg_opaque));
typedef void (*p_rd_kafka_conf_set_rebalance_cb)(
    rd_kafka_conf_t *conf,
    void (*rebalance_cb) (rd_kafka_t *rk,
    rd_kafka_resp_err_t err,
    rd_kafka_topic_partition_list_t *partitions,
    void *opaque));
typedef rd_kafka_resp_err_t (*p_rd_kafka_consumer_close)(rd_kafka_t *rk);
typedef rd_kafka_message_t* (*p_rd_kafka_consumer_poll)(rd_kafka_t *rk, int timeout_ms);
typedef rd_kafka_resp_err_t (*p_rd_kafka_last_error)(void);
typedef void (*p_rd_kafka_message_destroy)(rd_kafka_message_t *rkmessage);
//协议栈的dll导出函数部分，使用包装类封装起来
typedef int64_t (*p_rd_kafka_message_timestamp)(const rd_kafka_message_t *rkmessage,
    rd_kafka_timestamp_type_t *tstype);
typedef int (*p_rd_kafka_outq_len)(rd_kafka_t *rk);
typedef rd_kafka_resp_err_t (*p_rd_kafka_poll_set_consumer)(rd_kafka_t *rk);
typedef rd_kafka_resp_err_t (*p_rd_kafka_subscribe)(rd_kafka_t *rk,
    const rd_kafka_topic_partition_list_t *topics);
typedef rd_kafka_conf_res_t (*p_rd_kafka_topic_conf_set)(rd_kafka_topic_conf_t *conf,
    const char *name,
    const char *value,
    char *errstr, size_t errstr_size);
typedef void (*p_rd_kafka_topic_destroy)(rd_kafka_topic_t *rkt);
typedef rd_kafka_topic_t * (*p_rd_kafka_topic_new)(rd_kafka_t *rk, const char *topic,
    rd_kafka_topic_conf_t *conf);
typedef rd_kafka_topic_partition_t * (*p_rd_kafka_topic_partition_list_add)(rd_kafka_topic_partition_list_t *rktparlist,
    const char *topic, int32_t partition);
typedef void (*p_rd_kafka_topic_partition_list_destroy)(rd_kafka_topic_partition_list_t *rkparlist);
typedef rd_kafka_topic_partition_list_t *(*p_rd_kafka_topic_partition_list_new)(int size);
typedef rd_kafka_resp_err_t (*p_rd_kafka_topic_partition_list_set_offset)(
    rd_kafka_topic_partition_list_t *rktparlist,
    const char *topic, int32_t partition, int64_t offset);
typedef rd_kafka_resp_err_t (*p_rd_kafka_unsubscribe)(rd_kafka_t *rk);
typedef void (*p_rd_kafka_metadata_destroy)(const struct rd_kafka_metadata *metadata);
typedef int (*p_rd_kafka_produce)(rd_kafka_topic_t *rkt, int32_t partition,
    int msgflags,
    void *payload, size_t len,
    const void *key, size_t keylen,
    void *msg_opaque);
typedef rd_kafka_resp_err_t (*p_rd_kafka_metadata)(rd_kafka_t *rk, int all_topics,
    rd_kafka_topic_t *only_rkt,
    const struct rd_kafka_metadata **metadatap,
    int timeout_ms);

typedef void (*p_rd_kafka_conf_set_offset_commit_cb) (
    rd_kafka_conf_t *conf,
    void (*offset_commit_cb) (rd_kafka_t *rk,
    rd_kafka_resp_err_t err,
    rd_kafka_topic_partition_list_t *offsets,
    void *opaque));

typedef rd_kafka_resp_err_t
    (*p_rd_kafka_commit) (rd_kafka_t *rk, const rd_kafka_topic_partition_list_t *offsets,
    int async);
typedef rd_kafka_resp_err_t (*p_rd_kafka_seek) (rd_kafka_topic_t *rkt,
    int32_t partition,
    int64_t offset,
    int timeout_ms);
typedef void
(*p_rd_kafka_topic_partition_list_add_range) (rd_kafka_topic_partition_list_t
                                         *rktparlist,
                                         const char *topic,
                                         int32_t start, int32_t stop);

typedef int
(*p_rd_kafka_topic_partition_list_del) (rd_kafka_topic_partition_list_t *rktparlist,
				   const char *topic, int32_t partition);

typedef int
(*p_rd_kafka_topic_partition_list_del_by_idx) (
	rd_kafka_topic_partition_list_t *rktparlist,
	int idx);

typedef rd_kafka_topic_partition_list_t *
(*p_rd_kafka_topic_partition_list_copy) (const rd_kafka_topic_partition_list_t *src);

typedef rd_kafka_topic_partition_t *
(*p_rd_kafka_topic_partition_list_find) (rd_kafka_topic_partition_list_t *rktparlist,
				    const char *topic, int32_t partition);

class IRdKafka
{
public:
    static p_rd_kafka_version _version;
    static p_rd_kafka_version_str _version_str;
    static p_rd_kafka_brokers_add _brokers_add;
    static p_rd_kafka_conf_destroy _conf_destroy;
    static p_rd_kafka_conf_dup _conf_dup;
    static p_rd_kafka_conf_new _conf_new;
    static p_rd_kafka_conf_set _conf_set;
    static p_rd_kafka_conf_set_error_cb _conf_set_error_cb;
    static p_rd_kafka_conf_set_log_cb _conf_set_log_cb;
    static p_rd_kafka_conf_set_opaque _conf_set_opaque;
    static p_rd_kafka_conf_set_stats_cb _conf_set_stats_cb;
    static p_rd_kafka_conf_set_throttle_cb _conf_set_throttle_cb;
    static p_rd_kafka_destroy _destroy;
    static p_rd_kafka_err2str _err2str;
    static p_rd_kafka_new _new;
    static p_rd_kafka_poll _poll;
    static p_rd_kafka_set_log_level _set_log_level;
    static p_rd_kafka_topic_conf_new _topic_conf_new;
    static p_rd_kafka_topic_conf_destroy _topic_conf_destroy;
    static p_rd_kafka_topic_name _topic_name;
    static p_rd_kafka_assign _assign;
    static p_rd_kafka_conf_get _conf_get;
    static p_rd_kafka_conf_set_consume_cb _conf_set_consume_cb;
    static p_rd_kafka_conf_set_default_topic_conf _conf_set_default_topic_conf;
    static p_rd_kafka_conf_set_dr_cb _conf_set_dr_cb;
    static p_rd_kafka_conf_set_rebalance_cb _conf_set_rebalance_cb;
    static p_rd_kafka_consumer_close _consumer_close;
    static p_rd_kafka_consumer_poll _consumer_poll;
    static p_rd_kafka_last_error _last_error;
    static p_rd_kafka_message_destroy _message_destroy;
    static p_rd_kafka_message_timestamp _message_timestamp;
    static p_rd_kafka_outq_len _outq_len;
    static p_rd_kafka_poll_set_consumer _poll_set_consumer;
    static p_rd_kafka_subscribe _subscribe;
    static p_rd_kafka_topic_conf_set _topic_conf_set;
    static p_rd_kafka_topic_destroy _topic_destroy;
    static p_rd_kafka_topic_new _topic_new;
    static p_rd_kafka_topic_partition_list_add _topic_partition_list_add;
    static p_rd_kafka_topic_partition_list_destroy _topic_partition_list_destroy;
    static p_rd_kafka_topic_partition_list_new _topic_partition_list_new;
    static p_rd_kafka_topic_partition_list_set_offset _topic_partition_list_set_offset;
    static p_rd_kafka_unsubscribe _unsubscribe;
    static p_rd_kafka_produce _produce;
    static p_rd_kafka_metadata_destroy _metadata_destroy;
    static p_rd_kafka_metadata _metadata;
    static p_rd_kafka_conf_set_offset_commit_cb _conf_set_offset_commit_cb;
    static p_rd_kafka_commit _commit;
    static p_rd_kafka_topic_partition_list_add_range _topic_partition_list_add_range;
    static p_rd_kafka_topic_partition_list_del _topic_partition_list_del;
    static p_rd_kafka_topic_partition_list_del_by_idx _topic_partition_list_del_by_idx;
    static p_rd_kafka_topic_partition_list_copy _topic_partition_list_copy;
    static p_rd_kafka_topic_partition_list_find _topic_partition_list_find;
    static p_rd_kafka_seek _seek;
    static RD_INLINE const char* _message_errstr(const rd_kafka_message_t *rkmessage) {
        if (!rkmessage->err)
            return NULL;

        if (rkmessage->payload)
            return (const char *)rkmessage->payload;

        return _err2str(rkmessage->err);
    };
public:
    static BOOL32 LoadDll();
    static void ReleaseDll();
private:
    IRdKafka(){};
    static u32_ptr m_pDll;
};


#endif  //_MQKAFKARDFUNC_H_