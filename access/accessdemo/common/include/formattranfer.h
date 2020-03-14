/*========================================================================================
    模块名    ：formattranfer
    文件名    ：formattranfer.h
    相关文件  ：
    实现功能  ：格式转换
    作者      ：zhaojunxiang
    版权      ：<Copyright(C) 2003-2020 Suzhou Keda Technology Co., Ltd. All rights reserved.>
    -------------------------------------------------------------------------------------------
    修改记录：
    日期               版本              修改人             走读人              修改记录
    2019/12/24         V1.0        zhaojunxiang                         新建文件
    =========================================================================================*/
#ifndef _FORMATTRANFER_H
#define _FORMATTRANFER_H

///////////////////////////////////////////////////////////////////////  

//ascii 转 Unicode  
static std::wstring Acsi2WideByte(std::string& strascii)  
{  
    int widesize = MultiByteToWideChar (CP_ACP, 0, (char*)strascii.c_str(), -1, NULL, 0);  
    if (widesize == ERROR_NO_UNICODE_TRANSLATION)  
    {  
        throw std::exception("Invalid UTF-8 sequence.");  
    }  
    if (widesize == 0)  
    {  
        throw std::exception("Error in conversion.");  
    }  
    std::vector<wchar_t> resultstring(widesize);  
    int convresult = MultiByteToWideChar (CP_ACP, 0, (char*)strascii.c_str(), -1, &resultstring[0], widesize);  


    if (convresult != widesize)  
    {  
        throw std::exception("La falla!");  
    }  

    return std::wstring(&resultstring[0]);  
}  
//utf8 to Unicode
static wstring Utf82Unicode(const std::string &str)
{
    int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    wchar_t *pUnicode = (wchar_t *)malloc(sizeof(wchar_t) * nwLen);
    if (pUnicode)
    {
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, pUnicode, nwLen);
        wstring wstr = pUnicode;
        free(pUnicode);
        return wstr;
    }
    return NULL;
}

//Unicode to ASCII
static std::string WideByte2Acsi(const wstring &wstr)
{
    int naLen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    char *pAssii = (char *)malloc(sizeof(char) * naLen);
    if (pAssii)
    {
        WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, pAssii, naLen, nullptr, nullptr);
        std::string str = pAssii;
        free(pAssii);
        return str;
    }
    return NULL;
}

//Unicode 转 Utf8  

static std::string Unicode2Utf8(const std::wstring& widestring)  
{  
    int utf8size = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, NULL, 0, NULL, NULL);  
    if (utf8size == 0)  
    {  
        throw std::exception("Error in conversion.");  
    }  

    std::vector<char> resultstring(utf8size);  

    int convresult = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, &resultstring[0], utf8size, NULL, NULL);  

    if (convresult != utf8size)  
    {  
        throw std::exception("La falla!");  
    }  

    return std::string(&resultstring[0]);  
}  

//ascii 转 Utf8  

static std::string ASCII2UTF_8(std::string& strAsciiCode)  
{  
    std::string strRet("");  
    //先把 ascii 转为 unicode  
    std::wstring wstr = Acsi2WideByte(strAsciiCode);  
    //最后把 unicode 转为 utf8  
    strRet = Unicode2Utf8(wstr);  
    return strRet;  
}  

//uft8 转 ascii
//utf8->unicode->ascii
static std::string UTF_82ASCII(const std::string &str)
{
    return WideByte2Acsi(Utf82Unicode(str));
}

/*********************************/
#endif