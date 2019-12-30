/*========================================================================================
    模块名    ：accessproto
    文件名    ：accessproto.h
    相关文件  ：
    实现功能  ：通信库、解析库、telnet库、断链重连等库的连接
    作者      ：zhaojunxiang
    版权      ：<Copyright(C) 2003-2020 Suzhou Keda Technology Co., Ltd. All rights reserved.>
    -------------------------------------------------------------------------------------------
    修改记录：
    日期               版本              修改人             走读人              修改记录
    2019/12/18         V1.0        zhaojunxiang                         新建文件
    =========================================================================================*/
#pragma once
#ifndef _ACCESSPROTO_H
#define _ACCESSPROTO_H

#include "pingserver.h" //PING通用函数 
#include "kdcommoninterface.h"
#include "commonsaevent.h"
#include "commonsaproto.h"//透明数据 cpp怎么搞过来

#include "connectbase.h"//通信库 
#include "analyze.h"//数据解析 json/xml的封装实现，以及配置文件 .ini 的读取
#include "CCreateDump.h" //dump头文件
#include "formattranfer.h" //格式转换UTF-8 ASSII UNICODE
#include "telnetServer.h" //osp telnet服务
#include "interface_alive_fun.h"//断链重连

#include "logclass.h"

#endif