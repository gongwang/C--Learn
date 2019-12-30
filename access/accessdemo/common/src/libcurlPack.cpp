#include "libcurlPack.h"
#include <curl.h>

/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *

 ***************************************************************************/

using namespace std;


struct MemoryStruct {
    char *memory;
    size_t size;
};

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) 
{
    string data((const char*) ptr, (size_t) size * nmemb);

    *((stringstream*) stream) << data << endl;

    return size * nmemb;
}

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t retcode;
  curl_off_t nread;
 
  /* in real-world cases, this would probably get this data differently
     as this fread() stuff is exactly what the library already would do
     by default internally */ 
  retcode = fread(ptr, size, nmemb, (FILE*)stream);
 
  nread = (curl_off_t)retcode;
 
  fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
          " bytes from file\n", nread);
 
  return retcode;
}

//https get
std::string HttpsGet( std::string url, std::string header, std::string content )
{
    CURL *curl;
    CURLcode res;
    std::stringstream out;

    struct  curl_slist  *headerlist = NULL;
    static  const char buf[] = "Content-Type:application/json;charset=UTF-8";
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl)
    {
        if (header == "")
        {
            headerlist = curl_slist_append(headerlist, buf); //默认头
        }else   
        {
            headerlist = curl_slist_append(headerlist, header.c_str()); //头 参数
        }
        
        ////get
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); //url 参数
        curl_easy_setopt(curl, CURLOPT_POST, 0); // 0：get reqest 1：post   //3. 类型 参数
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

        curl_easy_setopt(curl, CURLOPT_SSLVERSION,CURL_SSLVERSION_TLSv1_2);// | CURL_SSLVERSION_SSLv3 | CURL_SSLVERSION_TLSv1_0 | CURL_SSLVERSION_TLSv1_2
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);//SKIP_PEER_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);//SKIP_HOSTNAME_VERFICATION
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 0);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); //设置返回数据回调
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out); //4. 内容回调
        res = curl_easy_perform(curl);
        if(res !=CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
        }

        std::string str_json = out.str();
        //printf("res: %s \n", str_json.c_str());

        curl_easy_cleanup(curl);
        curl_slist_free_all(headerlist);
        
        return str_json;
    }
    return "";
}

//https post
std::string HttpsPost( std::string url, std::string header,std::string content )
{
    CURL *curl;
    CURLcode res;
    std::stringstream out;

    struct  curl_slist  *headerlist = NULL;
    static  const char buf[] = "Content-Type:application/json;charset=UTF-8";
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl)
    {
        if (header == "")
        {
            headerlist = curl_slist_append(headerlist, buf); //默认头
        }else   
        {
            headerlist = curl_slist_append(headerlist, header.c_str()); //头 参数
        }

        ////get
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); //1. url 参数
        curl_easy_setopt(curl, CURLOPT_POST, 1); // 0：get reqest 1：post   //3. 类型 参数      
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS,  content.c_str()); //4.发送内容
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(content.c_str()));
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

        curl_easy_setopt(curl, CURLOPT_SSLVERSION,CURL_SSLVERSION_TLSv1_2);// | CURL_SSLVERSION_SSLv3 | CURL_SSLVERSION_TLSv1_0 | CURL_SSLVERSION_TLSv1_2
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);//SKIP_PEER_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);//SKIP_HOSTNAME_VERFICATION
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 0);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); //设置返回数据回调
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out); //4. 内容回调
        res = curl_easy_perform(curl);
        if(res !=CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
        }

        std::string str_json = out.str();
        //printf("res: %s \n", str_json.c_str());

        curl_easy_cleanup(curl);
        curl_slist_free_all(headerlist);

        return str_json;
    }
    return "";
}

//https put
std::string HttpsPut( std::string url, std::string header, std::string content )
{
    CURL *curl;
    CURLcode res;
    std::stringstream out;

    struct  curl_slist  *headerlist = NULL;
    static  const char buf[] = "Content-Type:application/json;charset=UTF-8";
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl)
    {
        if (header == "")
        {
            headerlist = curl_slist_append(headerlist, buf); //默认头
        }else   
        {
            headerlist = curl_slist_append(headerlist, header.c_str()); //头 参数
        }
        ////get
        FILE * hd_src;
        struct stat file_info;
        char szFilePath[MAX_PATH + 1]={0};
        GetModuleFileName(NULL, szFilePath, MAX_PATH);
        char* pEnd = strrchr(szFilePath, '\\');
        if(pEnd)
        {
            *pEnd = 0;
        }
        strcat(szFilePath, "\\httpsputtemp.ini");
        if (_access(szFilePath, NULL)) //只检查文件是否存在 不在的话 创建 然后关闭
        {
            FILE * temp = fopen(szFilePath, "w");
            fclose(temp);
        }
        char *file = szFilePath;
        hd_src = fopen(file, "rb");    
        stat(file, &file_info);
  
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); //1. url 参数
        curl_easy_setopt(curl, CURLOPT_PUT, 1); //    //3. 类型 参数
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        /* now specify which file to upload */ 
        curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
        /* provide the size of the upload, we specicially typecast the value
       to curl_off_t since we must be sure to use the correct data size */ 
       curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_info.st_size);
       //curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);

        curl_easy_setopt(curl, CURLOPT_SSLVERSION,CURL_SSLVERSION_TLSv1_2);// | CURL_SSLVERSION_SSLv3 | CURL_SSLVERSION_TLSv1_0 | CURL_SSLVERSION_TLSv1_2
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);//SKIP_PEER_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);//SKIP_HOSTNAME_VERFICATION
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 0);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); //设置返回数据回调
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out); //4. 内容回调
        res = curl_easy_perform(curl);
        if(res !=CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
        }

        std::string str_json = out.str();
        //printf("res: %s \n", str_json.c_str());

        curl_easy_cleanup(curl);
        curl_slist_free_all(headerlist);

        return str_json;
    }
    return "";
}

void HttpsReqAndRes(HTTPSmethods method, TData &tData)
{
    std::string url = tData.pUrl;
    std::string header = tData.header;
    std::string content = tData.content;
    std::string strRecv = "";

    if ( url  == "" )
    {
        printf("Url error, url:％s\n", url.c_str());
        return ;
    }

    if (method< HTTPSmethods::HTTPS_PUT || method > HTTPSmethods::HTTPS_GET)
    {
        printf("mothod [%d]error, func does not support this method \n", method);
        return ;
    }

    int nMethod = method;
   // std::string strRecv = "";

    switch(nMethod)
    {
    case HTTPS_GET:
        strRecv = HttpsGet(url, header, content);
        break;
    case HTTPS_POST:
        strRecv = HttpsPost(url, header, content);
        break;
    case HTTPS_PUT:
        strRecv = HttpsPut(url, header, content);
        break;
    default:
        break;
    }
    //printf("Https Recv: %s \n length: %d \n", strRecv.c_str(), strRecv.length());

    tData.pRcv = new TCHAR[strRecv.length() +1];

    //printf("malloc size :%d \n", strlen(tData.pRcv));

    if ( tData.pRcv == NULL)
    {
        printf("malloc 失败\n");
        return;
    }

    memcpy(tData.pRcv, strRecv.c_str(), strRecv.length() + 1);

    //strncpy(tData.pRcv, strRecv.c_str(), strlen(tData.pRcv));

   // printf("tData Recv: %s \n", tData.pRcv);

    return ;
}

void HttpsFreeDataRcv( TData &tData )
{
    if (tData.pRcv != NULL)
    {
       delete[] tData.pRcv;
    }
    return;
}

