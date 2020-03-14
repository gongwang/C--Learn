/*========================================================================================
模块名    ：dbix.lib
文件名    ：dbix_debug.h
相关文件  ：
实现功能  ：问题诊断功能。
作者      ：huzhiyun
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/06/05         V1.0              huzhiyun                               新建文件
=========================================================================================*/
#ifndef _DBIX_DEBUG_H_
#define _DBIX_DEBUG_H_

#include "dbix.h"
#include "osp.h"

DBIX_NS_BEGIN

// 日志打印函数，内部实现要求一次输出内容小于1K
// 注意，内部会显式追加'\n'
void log(TLogLvl tLogLvl, const s8* lpcstrFormat, ...);

// 日志输出等级控制，0表示禁用所有的，数值越大，允许的输出级别越多; 具体参见TLogLvl枚举
// API void dbixlog(u8 byLvl);
// 
// API void dbixstat();
// 
// API void dbixver();
// 
// API void dbixhelp();

DBIX_NS_END
#endif // _DBIX_DEBUG_H_
