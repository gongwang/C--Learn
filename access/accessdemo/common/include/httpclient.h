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

//��ʱû��
void SetDefaultHeader(std::map<std::string,std::string> header);

//֧���Զ�������ͷ
Response& HttpRequestSync(/*OUT*/ Response& response, const std:: string &content, const char* url,emMethod method= E_GET_METHOD,int nTimeout = 5000,std::map<std::string,std::string> header = std::map<std::string,std::string>());