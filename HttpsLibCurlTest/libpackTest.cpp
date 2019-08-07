/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *

 ***************************************************************************/

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <io.h>
#include "libcurlPack.h"

using namespace std;


int main(void)
{
    
    //get test
    /*
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
    */

    //post
    /*
    string url = "https://172.16.129.240:24408/syris/device/assign";
    string header = "";
    HTTPSmethods method = HTTPSmethods::HTTPS_POST;
    string content = "{\"controller_maps\":      [{\
                     \"sycg_identifier\":      \"0000000000000000\",\
                     \"alias\":        \"SY832-A-PV8\",\
                     \"connection\":   {\
                     \"mode\": 0,\
                     \"port\": 5001,\
                     \"address\":      2306937004,\
                     \"identification\":       0\
                     },\
                     \"apb\":  {\
                     \"group\":  0\
                     }\
                     }]\
                     }";

    printf("method %d \n", method);

    string recv  = "";

    TData tdata(url, header, content);

    memcpy(tdata.pUrl, url.c_str(), url.length() + 1);
    memcpy(tdata.header, header.c_str(), header.length() + 1);
    memcpy(tdata.content, content.c_str(), content.length() + 1);

    HttpsReqAndRes(method, tdata);

    printf("call back : %s  \n", tdata.pRcv);
    system("pause");
    */

    //put
    
    string url = "https://172.16.129.240:24408/syris/device";
    string header = "";
    HTTPSmethods method = HTTPSmethods::HTTPS_PUT;
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
}