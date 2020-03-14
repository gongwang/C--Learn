kafka消息队列封装库，运行依赖librdkafka库，pthread库和zlib库。
librdkafka库代码使用c99标准，需使用vs2013以上版本编译成动态库再给vs2010使用
librdkafka依赖libssl和libsasl，centos下使用需安装openssl-devel和cyrus-sasl-devel库
关于创建连接和发送器/接收器时的超时时间，远程时1s会出现获取group信息失败导致创建receiver失败问题

librdkafka 0.11.3编译问题：
如使用vs2015及以上版本，使用SASL需安装1.0.2n版本的openssl
且需手动引入legacy_stdio_definitions.lib静态库，
并定义
#if _MSC_VER>=1900
#ifndef _IOB_FUNC_INSTEAD_
#define _IOB_FUNC_INSTEAD_
#include "stdio.h" 
_ACRTIMP_ALT FILE* __cdecl __acrt_iob_func(unsigned);
#endif // !_IOB_FUNC_INSTEAD_
#endif /* _MSC_VER>=1900 */
，并在任何c源码文件内实现函数__iob_func
#if _MSC_VER>=1900
#ifdef __cplusplus 
extern "C"
#endif 
FILE* __cdecl __iob_func(unsigned i) {
	return __acrt_iob_func(i);
}
#endif /* _MSC_VER>=1900 */