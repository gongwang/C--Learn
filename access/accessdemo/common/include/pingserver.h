/*========================================================================================
    模块名    ：pingserver
    文件名    ：pingserver.h
    相关文件  ：
    实现功能  ：断链重连所使用的ping方式公共函数
    作者      ：zhaojunxiang
    版权      ：<Copyright(C) 2003-2020 Suzhou Keda Technology Co., Ltd. All rights reserved.>
    -------------------------------------------------------------------------------------------
    修改记录：
    日期               版本              修改人             走读人              修改记录
    2019/12/23         V1.0        zhaojunxiang                         新建文件
    =========================================================================================*/
#pragma once
#ifndef _PINGSERVER_H
#define _PINGSERVER_H

#include "pthread_w32.h"
//#include <process.h>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

//定义默认缓冲区长度
#define DEF_BUF_SIZE 1024
#define IP_HEADER_SIZE 20
#define ICMP_HEADER_SIZE (sizeof(ICMP_HEADER))
#define ICMP_DATA_SIZE 32
#define ICMP_PACK_SIZE (ICMP_HEADER_SIZE + ICMP_DATA_SIZE)

typedef struct _ICMP_HEADER{
    BYTE nType;
    BYTE nCode;
    USHORT nCheckSum;
    USHORT nId;
    USHORT nSequence;
    UINT nTimeStamp;
}ICMP_HEADER,*PICMP_HEADER;


static u32 StrToHexUL(const s8* pszStr)
{
    if(NULL == pszStr)
    {
        return -1;
    }

    s8 * pszStop;
    return strtoul(pszStr, &pszStop, 16);
}
static  std::string format( const char* lpszFormat, ... )
{
    va_list args;
    va_start(args, lpszFormat);

    char szBuffer[4096+1] = {0};

    vsnprintf_s(szBuffer, 4096, lpszFormat, args);
    perror(szBuffer);
    va_end(args);
    std::string strbuf;

    strbuf.assign(szBuffer);
    return strbuf;
}
static  u32 CalculateCheckSum(const u8* data, const s32 len)
{
    u32 dwSum = 0;
    u32 dwTmp = 0;
    std::string strTmp;
    for (s32 i = 0; i < len-1; i++)
    {
        strTmp = format("%02X", (u8)data[i]);
        dwTmp = StrToHexUL(strTmp.c_str());
        dwSum += dwTmp;
    }

    dwSum %= 256;
    return dwSum;
}

static  BOOL32 CheckSum(const u8* data, const s32 len)
{
    u32 dwSum = 0;
    u32 dwTmp = 0;
    std::string strTmp;
    for (s32 i = 0; i < len-1; i++)
    {
        strTmp = format("%02X", (u8)data[i]);
        dwTmp = StrToHexUL(strTmp.c_str());
        dwSum += dwTmp;
    }

    dwSum %= 256;
    return (dwSum == (u32)data[len-1]);
}

static  USHORT GetCheckSum(LPBYTE lpBuf, DWORD dwSize)
{
    DWORD dwCheckSum = 0;
    USHORT* lpWord = (USHORT*)lpBuf;

    while( dwSize > 1)
    {
        dwCheckSum += *lpWord++;
        dwSize -= 2;
    }

    if(1 == dwSize)
        dwCheckSum += *((USHORT*)lpWord);

    dwCheckSum = ( dwCheckSum >> 16) + ( dwCheckSum & 0xffff);
    return (USHORT)(~dwCheckSum);
}

static  BOOL Ping(char* lpDestIp)
{
    SOCKADDR_IN DestAddr;
    DestAddr.sin_family = AF_INET;
    DestAddr.sin_addr.S_un.S_addr = inet_addr(lpDestIp);
    DestAddr.sin_port = htons(0);

    //创建ICMP请求包
    char ICMPPack[ICMP_PACK_SIZE] = {0};
    PICMP_HEADER pICMPHeader = (PICMP_HEADER)ICMPPack;
    pICMPHeader->nType = 8;
    pICMPHeader->nCode = 0;
    pICMPHeader->nId = (USHORT)::GetCurrentProcessId();
    pICMPHeader->nCheckSum = 0;
    pICMPHeader->nTimeStamp = 0;
    memset(&(ICMPPack[ICMP_HEADER_SIZE]),'E',ICMP_DATA_SIZE);

    //初始化WinSock
    static BOOL32 s_bInitWinSock = FALSE;
    if(s_bInitWinSock == FALSE)
    {
        WORD wVersionRequested = MAKEWORD(2,2);
        WSADATA wsaData;
        if(WSAStartup(wVersionRequested,&wsaData) == 0)
        {
            s_bInitWinSock = TRUE;
            //return FALSE;
        }
    }

    //创建初始套接字
    SOCKET RawSock = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);
    if(INVALID_SOCKET == RawSock)
    {
        printf("create raw socket error\n");
        return FALSE;
    }

    int nTime = 1000;
    int nRet = ::setsockopt( RawSock, SOL_SOCKET, SO_RCVTIMEO,(char*)(&nTime),sizeof(nTime));

    char szRecvBuf [ DEF_BUF_SIZE] ;
    SOCKADDR_IN    SourSockAddr ;

    BOOL bRet = TRUE;
    for(int i = 0; i < 1; i++)
    {
        pICMPHeader->nCheckSum = 0;
        pICMPHeader->nTimeStamp = ::GetTickCount();
        pICMPHeader->nSequence = i;

        pICMPHeader->nCheckSum = GetCheckSum ( (LPBYTE)ICMPPack, ICMP_PACK_SIZE ) ;

        int nRet = ::sendto( RawSock, ICMPPack, ICMP_PACK_SIZE, 0, (SOCKADDR*)&DestAddr, sizeof(DestAddr));
        if ( nRet == SOCKET_ERROR )
        {
            printf( "sendto error!\n" ) ;
            bRet = FALSE ;
            break;
        }

        // 接收ICMP响应
        int nLen = sizeof(SourSockAddr) ;
        nRet = ::recvfrom ( RawSock, szRecvBuf, DEF_BUF_SIZE,0,(SOCKADDR*)&SourSockAddr, &nLen ) ;
        if ( nRet == SOCKET_ERROR )
        {
            if ( ::WSAGetLastError() == WSAETIMEDOUT )
            {
                printf ( "Request Timeout\n" ) ;
                bRet = FALSE;
                continue ;
            }
            else
            {
                printf ( "recvfrom error!\n" ) ;
                bRet = FALSE ;
                break;
            }
        }

        int nTime = ::GetTickCount() - pICMPHeader->nTimeStamp ;

        int nRealSize = nRet - IP_HEADER_SIZE - ICMP_HEADER_SIZE ;
        if ( nRealSize < 0  )
        {
            printf ( "To less recv bytes!\n" ) ;
            continue ;
        }

        // 检测是否当前所发出的ICMP响应包
        PICMP_HEADER pRecvHeader = (PICMP_HEADER)(szRecvBuf+IP_HEADER_SIZE) ;
        if ( pRecvHeader->nType != 0 )
        {
            printf ( "Not ICMP respond type!\n" ) ;
            bRet = FALSE ;
            break;
        }

        if ( pRecvHeader->nId != ::GetCurrentProcessId () )
        {
            printf ( "not valid id!\n" ) ;
            bRet = FALSE ;
            break;
        }

        printf ( "%d bytes replay from %s : bytes=%d time=%dms\n", \
            nRet, inet_ntoa(SourSockAddr.sin_addr), nRealSize, nTime ) ;

        //::Sleep ( 1000 ) ;
    }

    closesocket ( RawSock ) ;
    //WSACleanup () ;

    return bRet ;
}
#endif