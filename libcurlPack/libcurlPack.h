/************************************************************************/
/* libcurlPack.lib libcurlPack.dll libcurlPack.h 
author:zhaojunxiang@kedacom.com
time: 2019年8月7日11:00:12
version：1.0
info：
    pack libcurl，support ：
    uDICT, FILE, FTP, FTPS, Gopher, HTTP, HTTPS, IMAP, IMAPS, LDAP, LDAPS, POP3, POP3S, RTMP, 
    RTSP, SCP, SFTP, SMB, SMBS, SMTP, SMTPS, Telnet and TFTP. curl supports SSL certificates, 
    HTTP POST, HTTP PUT, FTP uploading, HTTP form based upload, proxies, HTTP/2, cookies, 
    user+password authentication (Basic, Plain, Digest, CRAM-MD5, NTLM, Negotiate and Kerberos), 
    file transfer resume, proxy tunneling and more.

    more information: https://curl.haxx.se/
*/
/************************************************************************/

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <io.h>
#include <tchar.h>

#define CURL_PACK_EXTERN  __declspec(dllimport)

#ifdef LIBCURLPACK_EXPORTS
#define MYDLL_API __declspec(dllexport)//注意decl前面是两个下划线
#else
#define MYDLL_API __declspec(dllimport)
#endif


enum HTTPSmethods
{
    HTTPS_PUT = 0,
    HTTPS_POST,
    HTTPS_GET,
};

//header 默认头：Content-Type:application/json;charset=UTF-8

struct TData
{
    TData(std::string url, std::string header, std::string content)
    {
        TData::pUrl = new TCHAR[url.length() +1];
        TData::header = new TCHAR[header.length() +1];
        TData::content = new TCHAR[content.length() +1];
        TData::pRcv = NULL;
    }
    ~TData()
    {
        delete[] TData::pUrl;
        delete[] TData::header;
        delete[] TData::content;
    }
    TCHAR* pUrl; //URL
    TCHAR* header; //HEDAER
    TCHAR* content; //CONTENT
    TCHAR* pRcv; //return data from network
};
//https send and recv func
MYDLL_API void HttpsReqAndRes(HTTPSmethods method, TData &tData);

//https clean tdata because it had been malloc in HttpsReqAndRes
MYDLL_API void HttpsFreeDataRcv(TData &tData);


//示例
/*
int main(void)
{

    //get test
    string url = "https://172.16.129.240:24408/syris/device/topology";
    string header = "";
    HTTPSmethods method = HTTPSmethods::HTTPS_GET;
    string content = "";


    printf("method %d \n", method);

    string recv  = "";

    TData tdata(url, header, content);

    memcpy(tdata.pUrl, url.c_str(), url.length() + 1);
    memcpy(tdata.header, header.c_str(), header.length() + 1);
    memcpy(tdata.content, content.c_str(), content.length() + 1);

    HttpsReqAndRes(method, tdata);

    printf("call back : %s  \n", tdata.pRcv);
    system("pause");

    //
    HttpsFreeDataRcv(tdata);

    return 0;
}*/