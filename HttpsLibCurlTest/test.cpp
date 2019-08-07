//
//#include <stdio.h>
//#include <curl.h>
//#include <string>
//
//CURL_EXTERN CURL *curl_easy_init(void);
//CURL_EXTERN CURLcode curl_easy_setopt(CURL *curl, CURLoption option, ...);
//CURL_EXTERN CURLcode curl_easy_perform(CURL *curl);
//CURL_EXTERN void curl_easy_cleanup(CURL *curl);
//
//typedef CURLcode (*g_curl_global_init)(long flags);
//typedef CURL* (*g_curl_easy_init)(void);
//typedef CURLcode (*g_curl_easy_setopt)(CURL *curl, CURLoption option, ...);
//typedef CURLcode (*g_curl_easy_perform)(CURL *curl);
//typedef void (*g_curl_easy_cleanup)(CURL *curl);
//
//g_curl_global_init G_curl_global_init = NULL;
//g_curl_easy_init G_curl_easy_init = NULL;
//g_curl_easy_setopt  G_curl_easy_setopt= NULL;
//g_curl_easy_perform G_curl_easy_perform = NULL;
//g_curl_easy_cleanup G_curl_easy_cleanup = NULL;
//
//int main()
//{
//
//    char szFilePath[MAX_PATH + 1]={0};
//    GetModuleFileName(NULL, szFilePath, MAX_PATH);
//    char* pEnd = strrchr(szFilePath, '\\');
//    if(pEnd)
//    {
//        *pEnd = 0;
//    }
//    strcat(szFilePath, "\\libcurl.dll");
//    //strcat(szFilePath, "E:\\kedacom-code\\ismp_vob\\40-csp\\speak\\speakmeiyi\\Debug\\speakmeiyi.dll");
//
//    HMODULE hModule = LoadLibrary(szFilePath);
//    if(hModule == NULL)
//    {
//        printf("load library failed.err:%d path:%s\n", GetLastError(), szFilePath);
//        system("pause");
//        return 0;
//    }
//
//    G_curl_global_init = (g_curl_global_init)GetProcAddress(hModule, "curl_global_init");
//    G_curl_easy_init = (g_curl_easy_init)GetProcAddress(hModule, "curl_easy_init");
//    G_curl_easy_setopt     = (g_curl_easy_setopt)GetProcAddress(hModule, "curl_easy_setopt");
//    G_curl_easy_perform   = (g_curl_easy_perform )GetProcAddress(hModule, "curl_easy_perform");
//    G_curl_easy_cleanup     = (g_curl_easy_cleanup)GetProcAddress(hModule, "curl_easy_cleanup");
//
//    if(!G_curl_global_init || !G_curl_easy_init || !G_curl_easy_setopt || !G_curl_easy_perform || !G_curl_easy_cleanup )
//    {
//        printf("get some fun failed.\n");
//        system("pause");
//        return 0;
//    }
//
//
//    CURL *curl;
//    CURLcode res;
//
//    G_curl_global_init(CURL_GLOBAL_DEFAULT);
//    //postman GET
//    std::string resPonseInfo = "";
//
//    CURL *hnd = G_curl_easy_init();
//
//    G_curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
//    G_curl_easy_setopt(hnd, CURLOPT_URL, "https://172.16.129.240:24408/syris/device/topology");
//
//    struct curl_slist *headers = NULL;
//    //headers = curl_slist_append(headers, "Postman-Token: 2899c86e-3ef1-4d6b-8394-0da2d36b1fd3");
//    //headers = curl_slist_append(headers, "cache-control: no-cache");
//    //G_curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
//
//    G_curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1);     //可以看到调试信息
//
//    G_curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, FALSE);
//    G_curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, FALSE);
//    G_curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &resPonseInfo);
//
//    res = G_curl_easy_perform(hnd);
//
//    //////////////////////////////////////////////////////////////////////////
//
//
//    /* Check for errors */
//    if(res != CURLE_OK)
//        fprintf(stderr, "curl_easy_perform() failed: %s\n",
//        /*curl_easy_strerror(res)*/"null");
//    printf("response: %s \n", resPonseInfo.c_str());
//    /* always cleanup */
//
//    system("pause");
//
//    G_curl_easy_cleanup(hnd);
//
//    //curl_global_cleanup();
//
//    system("pause");
//
//
//    return 0;
//}


//////////////////////////////////////////////////////////////////////////
//  curl = curl_easy_init();
//  if(curl) {
//    //curl_easy_setopt(curl, CURLOPT_URL, "https://www.baidu.com/");
//    curl_easy_setopt(curl, CURLOPT_URL, "https://172.16.129.240:24408/syris/device/assign/");
//
//#ifdef SKIP_PEER_VERIFICATION
//    /*
//     * If you want to connect to a site who isn't using a certificate that is
//     * signed by one of the certs in the CA bundle you have, you can skip the
//     * verification of the server's certificate. This makes the connection
//     * A LOT LESS SECURE.
//     *
//     * If you have a CA cert for the server stored someplace else than in the
//     * default bundle, then the CURLOPT_CAPATH option might come handy for
//     * you.
//     */
//    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
//#endif
//
//#ifdef SKIP_HOSTNAME_VERIFICATION
//    /*
//     * If the site you're connecting to uses a different host name that what
//     * they have mentioned in their server certificate's commonName (or
//     * subjectAltName) fields, libcurl will refuse to connect. You can skip
//     * this check, but this will make the connection less secure.
//     */
//    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
//#endif
//
//    char* postdata = "{\
//                     \"controller_maps\":      [{\
//                     \"sycg_identifier\":      \"0000000000000000\",\
//                     \"alias\":        \"SY832-A-PV8\",\
//                     \"connection\":   {\
//                     \"mode\": 0,\
//                     \"port\": 5001,\
//                     \"address\":      2306937004,\
//                     \"identification\":       0\
//                     },\
//                     \"apb\":  {\
//                     \"group\":        0\
//                     }\
//                     }]\
//                     }";
//
//    char* header = "application/json;charset=utf-8";
//    std::string resPonseInfo = "";
//
//    curl_easy_setopt(curl, CURLOPT_POST, 1L);
//    curl_easy_setopt(curl,CURLOPT_POSTFIELDS, (char *)postdata);
//    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postdata));
//    curl_easy_setopt(curl,CURLOPT_HEADERFUNCTION, (char *)header);
//    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resPonseInfo);
//    //curl_easy_setopt(curl, CURLOPT_PROXY, "http://proxy:80");
//    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,1); 
//
//    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);     //可以看到调试信息
//
//   // curl_easy_setopt (curl,CURLOPT_SSLCERT,"D:\\VS2012_Project\\HttpsLibCurlTest\\SDK\\service.crt");
//    //curl_easy_setopt (curl,CURLOPT_SSLCERT,"C:/SYRIS/sycg/etc/service.crt");
//    
//    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0); //不使用数字证书
//    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
//    
//    curl_easy_setopt (curl,CURLOPT_TIMEOUT, 60L);
//
//    curl_easy_setopt (curl,CURLOPT_CONNECTTIMEOUT, 10L);
//
//    res = curl_easy_perform(curl);


//  /* Check for errors */
//  if(res != CURLE_OK)
//    fprintf(stderr, "curl_easy_perform() failed: %s\n",
//            curl_easy_strerror(res));
//  //printf("response: %s \n", resPonseInfo.c_str());
//  /* always cleanup */
//  curl_easy_cleanup(curl);
//}

//  //////////////////////////////////////////////////////////////////////////
////postman POST
//std::string resPonseInfo = "";
//CURL *hnd = curl_easy_init();

//curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
//curl_easy_setopt(hnd, CURLOPT_URL, "https://172.16.129.240:24408/syris/device/assign");

//struct curl_slist *headers = NULL;
//headers = curl_slist_append(headers, "Postman-Token: 0105f58d-3c1c-4e07-8f40-bcf80d4c3b97");
//headers = curl_slist_append(headers, "cache-control: no-cache");
//headers = curl_slist_append(headers, "Content-Type: application/json");

//curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1);     //可以看到调试信息

//curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0);
//curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0);

//curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &resPonseInfo);

//curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

//curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, "{\r\n        \"controller_maps\":      [{\r\n                        \"sycg_identifier\":      \"0000000000000000\",\r\n                        \"alias\":        \"SY832-A-PV8\",\r\n                        \"connection\":   {\r\n                                \"mode\": 0,\r\n                                \"port\": 5001,\r\n                                \"address\":      2306937004,\r\n                                \"identification\":       0\r\n                        },\r\n                        \"apb\":  {\r\n                                \"group\":        0\r\n                        }\r\n                }]\r\n}");

//res = curl_easy_perform(hnd);

////////////////////////////////////////////////////////////////////////////