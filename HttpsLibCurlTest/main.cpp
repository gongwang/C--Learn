///***************************************************************************
// *                                  _   _ ____  _
// *  Project                     ___| | | |  _ \| |
// *                             / __| | | | |_) | |
// *                            | (__| |_| |  _ <| |___
// *                             \___|\___/|_| \_\_____|
// *
//
// ***************************************************************************/
//
//#include <stdio.h>
//#include <curl.h>
//#include <string>
//#include <iostream>
//#include <sstream>
//#include <string>
//#include <sys/stat.h>
//#include <io.h>
//using namespace std;
////#pragma comment ( lib, "ws2_32.lib" )
////#pragma comment ( lib, "winmm.lib" )
//
//struct MemoryStruct {
//    char *memory;
//    size_t size;
//};
//
//size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) 
//{
//    string data((const char*) ptr, (size_t) size * nmemb);
//
//    *((stringstream*) stream) << data << endl;
//
//    return size * nmemb;
//}
//
//static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
//{
//  size_t retcode;
//  curl_off_t nread;
// 
//  /* in real-world cases, this would probably get this data differently
//     as this fread() stuff is exactly what the library already would do
//     by default internally */ 
//  retcode = fread(ptr, size, nmemb, (FILE*)stream);
// 
//  nread = (curl_off_t)retcode;
// 
//  fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
//          " bytes from file\n", nread);
// 
//  return retcode;
//}
//
//int main(void)
//{
//    CURL *curl;
//    CURLcode res;
//    std::stringstream out;
//
//    struct  curl_slist  *headerlist = NULL;
//    static  const char buf[] = "Content-Type:application/json;charset=UTF-8";
//    curl_global_init(CURL_GLOBAL_ALL);
//    curl = curl_easy_init();
//    if(curl)
//    {
//        headerlist = curl_slist_append(headerlist, buf); //2.头 参数
//
//        //curl_easy_setopt(curl, CURLOPT_PUT, 0); // put  //3. 类型 参数   
//        
//
//
//        const char* postdata = "{\"controller_maps\":      [{\
//                               \"sycg_identifier\":      \"0000000000000000\",\
//                               \"alias\":        \"SY832-A-PV8\",\
//                               \"connection\":   {\
//                               \"mode\": 0,\
//                               \"port\": 5001,\
//                               \"address\":      2306937004,\
//                               \"identification\":       0\
//                               },\
//                               \"apb\":  {\
//                               \"group\":  0\
//                               }\
//                               }]\
//                               }";
//
//        ////get
//        //curl_easy_setopt(curl, CURLOPT_URL, "https://172.16.129.240:24408/syris/device/topology"); //1. url 参数
//        //curl_easy_setopt(curl, CURLOPT_POST, 0); // 0：get reqest 1：post   //3. 类型 参数
//        //curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
//        
//        //Post
//        //curl_easy_setopt(curl, CURLOPT_URL, "https://172.16.129.240:24408/syris/device/assign"); //1. url 参数
//        //curl_easy_setopt(curl, CURLOPT_POST, 1); // 0：get reqest 1：post   //3. 类型 参数      
//        //curl_easy_setopt(curl, CURLOPT_POSTFIELDS,  postdata); //4.发送内容
//        //curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postdata));
//        //curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
//        //put
//        FILE * hd_src;
//        struct stat file_info;
//        char szFilePath[MAX_PATH + 1]={0};
//        GetModuleFileName(NULL, szFilePath, MAX_PATH);
//        char* pEnd = strrchr(szFilePath, '\\');
//        if(pEnd)
//        {
//            *pEnd = 0;
//        }
//        strcat(szFilePath, "\\httpsputtemp.ini");
//        if (_access(szFilePath, NULL)) //只检查文件是否存在 不在的话 创建 然后关闭
//        {
//            FILE * temp = fopen(szFilePath, "w");
//            fclose(temp);
//        }
//        char *file = szFilePath;
//        hd_src = fopen(file, "rb");    
//        stat(file, &file_info);
//  
//        curl_easy_setopt(curl, CURLOPT_URL, "https://172.16.129.240:24408/syris/device"); //1. url 参数
//        curl_easy_setopt(curl, CURLOPT_PUT, 1); //    //3. 类型 参数
//        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
//        /* now specify which file to upload */ 
//        curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
//        /* provide the size of the upload, we specicially typecast the value
//       to curl_off_t since we must be sure to use the correct data size */ 
//       curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_info.st_size);
//       //curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
//
//        curl_easy_setopt(curl, CURLOPT_SSLVERSION,CURL_SSLVERSION_TLSv1_2);// | CURL_SSLVERSION_SSLv3 | CURL_SSLVERSION_TLSv1_0 | CURL_SSLVERSION_TLSv1_2
//        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);//SKIP_PEER_VERIFICATION
//        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);//SKIP_HOSTNAME_VERFICATION
//        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
//        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 0);
//        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
//        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); //设置返回数据回调
//        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out); //4. 内容回调
//        res = curl_easy_perform(curl);
//        if(res !=CURLE_OK)
//        {
//            fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
//        }
//
//        std::string str_json = out.str();
//        printf("res: %s \n", str_json.c_str());
//
//        curl_easy_cleanup(curl);
//        curl_slist_free_all(headerlist);
//    }
//    system("pause");
//    return 0;
//}