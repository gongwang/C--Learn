#pragma once
#include "sim_parser.h"
#include <set>
#include <map>

enum emMethod
{
	E_GET_METHOD = 0,
	E_POST_METHOD,
	E_PUT_METHOD,
	E_DELETE_METHOD
};

//暂时没用
void SetDefaultHeader(std::map<std::string,std::string> header);

//支持自定义请求头
Response& HttpRequestSync(/*OUT*/ Response& response, const std:: string &content, const char* url,emMethod method= E_GET_METHOD,int nTimeout = 5000,std::map<std::string,std::string> header = std::map<std::string,std::string>());