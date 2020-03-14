/*====================================================================
模块名 ：mvredis
文件名 ：hiredisref.h
相关文件：hiredis.h
实现功能：hiredis接口函数指针定义，用于动态载入hiredis库
作者 ：刘春光
版权 ：<Copyright)(C) 2003-2018 Suzhou Keda Technology Co., Ltd. All rights reserved.>
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2018/1/8       V1.0         刘春光
====================================================================*/
#pragma once
#ifndef _MCREDIS_HIREDIS_DEF_H_
#define _MCREDIS_HIREDIS_DEF_H_
//#include "hiredis.h"
//************************************************
#include <stdio.h> /* for size_t */
#include <stdarg.h> /* for va_list */
#ifndef _WIN32
#include <sys/time.h> /* for struct timeval */
#else
#include "win32_types_hiredis.h"
#endif

#ifndef _WIN32
#define PORT_LONGLONG long long
#endif

#define HIREDIS_MAJOR 0
#define HIREDIS_MINOR 11
#define HIREDIS_PATCH 0

#define REDIS_ERR -1
#define REDIS_OK 0

/* When an error occurs, the err flag in a context is set to hold the type of
 * error that occured. REDIS_ERR_IO means there was an I/O error and you
 * should use the "errno" variable to find out what is wrong.
 * For other values, the "errstr" field will hold a description. */
#define REDIS_ERR_IO 1 /* Error in read or write */
#define REDIS_ERR_EOF 3 /* End of file */
#define REDIS_ERR_PROTOCOL 4 /* Protocol error */
#define REDIS_ERR_OOM 5 /* Out of memory */
#define REDIS_ERR_OTHER 2 /* Everything else... */

/* Connection type can be blocking or non-blocking and is set in the
 * least significant bit of the flags field in redisContext. */
#define REDIS_BLOCK 0x1

/* Connection may be disconnected before being free'd. The second bit
 * in the flags field is set when the context is connected. */
#define REDIS_CONNECTED 0x2

/* The async API might try to disconnect cleanly and flush the output
 * buffer and read all subsequent replies before disconnecting.
 * This flag means no new commands can come in and the connection
 * should be terminated once all replies have been read. */
#define REDIS_DISCONNECTING 0x4

/* Flag specific to the async API which means that the context should be clean
 * up as soon as possible. */
#define REDIS_FREEING 0x8

/* Flag that is set when an async callback is executed. */
#define REDIS_IN_CALLBACK 0x10

/* Flag that is set when the async context has one or more subscriptions. */
#define REDIS_SUBSCRIBED 0x20

/* Flag that is set when monitor mode is active */
#define REDIS_MONITORING 0x40

#define REDIS_REPLY_STRING 1
#define REDIS_REPLY_ARRAY 2
#define REDIS_REPLY_INTEGER 3
#define REDIS_REPLY_NIL 4
#define REDIS_REPLY_STATUS 5
#define REDIS_REPLY_ERROR 6

#define REDIS_READER_MAX_BUF (1024*16)  /* Default max unused reader buffer. */

#define REDIS_KEEPALIVE_INTERVAL 15 /* seconds */

/* This is the reply object returned by redisCommand() */
typedef struct redisReply {
    int type; /* REDIS_REPLY_* */
    PORT_LONGLONG integer; /* The integer when type is REDIS_REPLY_INTEGER */
    int len; /* Length of string */
    char *str; /* Used for both REDIS_REPLY_ERROR and REDIS_REPLY_STRING */
    size_t elements; /* number of elements, for REDIS_REPLY_ARRAY */
    struct redisReply **element; /* elements vector for REDIS_REPLY_ARRAY */
} redisReply;

typedef struct redisReadTask {
    int type;
    int elements; /* number of elements in multibulk container */
    int idx; /* index in parent (array) object */
    void *obj; /* holds user-generated value for a read task */
    struct redisReadTask *parent; /* parent task */
    void *privdata; /* user-settable arbitrary field */
} redisReadTask;

typedef struct redisReplyObjectFunctions {
    void *(*createString)(const redisReadTask*, char*, size_t);
    void *(*createArray)(const redisReadTask*, int);
    void *(*createInteger)(const redisReadTask*, PORT_LONGLONG);
    void *(*createNil)(const redisReadTask*);
    void (*freeObject)(void*);
} redisReplyObjectFunctions;

/* State for the protocol parser */
typedef struct redisReader {
    int err; /* Error flags, 0 when there is no error */
    char errstr[128]; /* String representation of error when applicable */

    char *buf; /* Read buffer */
    size_t pos; /* Buffer cursor */
    size_t len; /* Buffer length */
    size_t maxbuf; /* Max length of unused buffer */

    redisReadTask rstack[9];
    int ridx; /* Index of current read task */
    void *reply; /* Temporary reply pointer */

    redisReplyObjectFunctions *fn;
    void *privdata;
} redisReader;

/* Context for a connection to Redis */
typedef struct redisContext {
    int err; /* Error flags, 0 when there is no error */
    char errstr[128]; /* String representation of error when applicable */
    int fd;
    int flags;
    char *obuf; /* Write buffer */
    redisReader *reader; /* Protocol reader */
} redisContext;
//HIREDIS_API定义
#  define HIREDIS_API //__stdcall
//************************************************
extern "C"{
typedef redisReader * (HIREDIS_API *predisReaderCreate)(void);
typedef void (HIREDIS_API *predisReaderFree)(redisReader *r);
typedef int (HIREDIS_API *predisReaderFeed)(redisReader *r, const char *buf, size_t len);
typedef int (HIREDIS_API *predisReaderGetReply)(redisReader *r, void **reply);

typedef void (HIREDIS_API *pfreeReplyObject)(void *reply);

typedef int (HIREDIS_API *predisvFormatCommand)(char **target, const char *format, va_list ap);
typedef int (HIREDIS_API *predisFormatCommand)(char **target, const char *format, ...);
typedef int (HIREDIS_API *predisFormatCommandArgv)(char **target, int argc, const char **argv, const size_t *argvlen);

typedef redisContext * (HIREDIS_API *predisConnect)(const char *ip, int port);
typedef redisContext * (HIREDIS_API *predisConnectWithTimeout)(const char *ip, int port, const struct timeval tv);
typedef redisContext * (HIREDIS_API *predisConnectNonBlock)(const char *ip, int port);
typedef redisContext * (HIREDIS_API *predisConnectBindNonBlock)(const char *ip, int port, const char *source_addr);
typedef redisContext * (HIREDIS_API *predisConnectUnix)(const char *path);
typedef redisContext * (HIREDIS_API *predisConnectUnixWithTimeout)(const char *path, const struct timeval tv);
typedef redisContext * (HIREDIS_API *predisConnectUnixNonBlock)(const char *path);
typedef redisContext * (HIREDIS_API *predisConnectFd)(int fd);
typedef int (HIREDIS_API *predisSetTimeout)(redisContext *c, const struct timeval tv);
typedef int (HIREDIS_API *predisEnableKeepAlive)(redisContext *c);
typedef void (HIREDIS_API *predisFree)(redisContext *c);
typedef int (HIREDIS_API *predisFreeKeepFd)(redisContext *c);
typedef int (HIREDIS_API *predisBufferRead)(redisContext *c);
typedef int (HIREDIS_API *predisBufferWrite)(redisContext *c, int *done);

typedef int (HIREDIS_API *predisGetReply)(redisContext *c, void **reply);
typedef int (HIREDIS_API *predisGetReplyFromReader)(redisContext *c, void **reply);

typedef int (HIREDIS_API *predisAppendFormattedCommand)(redisContext *c, const char *cmd, size_t len);

typedef int (HIREDIS_API *predisvAppendCommand)(redisContext *c, const char *format, va_list ap);
typedef int (HIREDIS_API *predisAppendCommand)(redisContext *c, const char *format, ...);
typedef int (HIREDIS_API *predisAppendCommandArgv)(redisContext *c, int argc, const char **argv, const size_t *argvlen);

typedef void * (HIREDIS_API *predisvCommand)(redisContext *c, const char *format, va_list ap);
typedef void * (HIREDIS_API *predisCommand)(redisContext *c, const char *format, ...);
typedef void * (HIREDIS_API *predisCommandArgv)(redisContext *c, int argc, const char **argv, const size_t *argvlen);
};
//将动态库中的函数声明为全局函数
extern predisReaderCreate g_predisReaderCreate;
extern predisReaderFree g_predisReaderFree ;
extern predisReaderFeed g_predisReaderFeed ;
extern predisReaderGetReply g_predisReaderGetReply ;

extern pfreeReplyObject g_pfreeReplyObject ;

extern predisvFormatCommand g_predisvFormatCommand ;
extern predisFormatCommand g_predisFormatCommand ;
extern predisFormatCommandArgv g_predisFormatCommandArgv ;

extern predisConnect g_predisConnect ;
extern predisConnectWithTimeout g_predisConnectWithTimeout ;
extern predisConnectNonBlock g_predisConnectNonBlock ;
extern predisConnectBindNonBlock g_predisConnectBindNonBlock ;
extern predisConnectUnix g_predisConnectUnix ;
extern predisConnectUnixWithTimeout g_predisConnectUnixWithTimeout ;
extern predisConnectUnixNonBlock g_predisConnectUnixNonBlock ;
extern predisConnectFd g_predisConnectFd ;
extern predisSetTimeout g_predisSetTimeout ;
extern predisEnableKeepAlive g_predisEnableKeepAlive ;
extern predisFree g_predisFree ;
extern predisFreeKeepFd g_predisFreeKeepFd ;
extern predisBufferRead g_predisBufferRead ;
extern predisBufferWrite g_predisBufferWrite ;

extern predisGetReply g_predisGetReply ;
extern predisGetReplyFromReader g_predisGetReplyFromReader ;

extern predisAppendFormattedCommand g_predisAppendFormattedCommand ;

extern predisvAppendCommand g_predisvAppendCommand ;
extern predisAppendCommand g_predisAppendCommand ;
extern predisAppendCommandArgv g_predisAppendCommandArgv ;

extern predisvCommand g_predisvCommand ;
extern predisCommand g_predisCommand ;
extern predisCommandArgv g_predisCommandArgv ;
#endif  //_MCREDIS_HIREDIS_DEF_H_