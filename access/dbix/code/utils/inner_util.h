/*========================================================================================
模块名    ：dbix.lib
文件名    ：inner_util.h
相关文件  ：
实现功能  ：dbix内部的工具类实现。
作者      ：huzhiyun
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/06/08         V1.0              huzhiyun                              新建文件
=========================================================================================*/
#ifndef _INNER_UTIL_H_
#define _INNER_UTIL_H_
#include "dbix.h"

DBIX_NS_BEGIN

void _replace_str(string& str, const string& strSrc, const string& strDst);

DBIX_NS_END
#endif // _INNER_UTIL_H_
