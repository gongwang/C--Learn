/*========================================================================================
ģ����    ��dbix.lib
�ļ���    ��dbix_debug.h
����ļ�  ��
ʵ�ֹ���  ��������Ϲ��ܡ�
����      ��huzhiyun
��Ȩ      ��<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/06/05         V1.0              huzhiyun                               �½��ļ�
=========================================================================================*/
#ifndef _DBIX_DEBUG_H_
#define _DBIX_DEBUG_H_

#include "dbix.h"
#include "osp.h"

DBIX_NS_BEGIN

// ��־��ӡ�������ڲ�ʵ��Ҫ��һ���������С��1K
// ע�⣬�ڲ�����ʽ׷��'\n'
void log(TLogLvl tLogLvl, const s8* lpcstrFormat, ...);

// ��־����ȼ����ƣ�0��ʾ�������еģ���ֵԽ��������������Խ��; ����μ�TLogLvlö��
// API void dbixlog(u8 byLvl);
// 
// API void dbixstat();
// 
// API void dbixver();
// 
// API void dbixhelp();

DBIX_NS_END
#endif // _DBIX_DEBUG_H_
