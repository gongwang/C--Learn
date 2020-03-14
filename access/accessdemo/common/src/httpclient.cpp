#include "httpclient.h"
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "multipart_parser.h"
#ifdef _MSC_VER
#include <winsock2.h>
#include <ws2tcpip.h>
#define snprintf _snprintf
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/select.h>
#include <netinet/in.h>
#endif
#define SS_READ_BUFFER_SIZE     4096
#define CLI_HOST_BUFFER_SIZE    256
#define CLI_PORT_BUFFER_SIZE    10

struct HttpClientData{
	Response *response;
	std::string lastField;

};

bool g_bIsCompelete;

//http协议文件传输转换-multipart_parser回调部分
int cli_split_str(const std::string &input, const char s, std::vector<std::string> &tokens) {
    if (input.empty()) {
        return 0;
    }
    std::stringstream ss;
    ss << input;
    std::string token;
    while (std::getline(ss, token, s)) {
        tokens.push_back(token);
    }
    return 0;
}

int cli_on_multipart_name(multipart_parser* p, const char *at, size_t length) {
    std::string s;
    s.assign(at, length);
    Response *rsp = (Response *)multipart_parser_get_data(p);
    std::vector<FileItem> *items = rsp->get_file_items();
    if (s == "Content-Disposition") {
        FileItem item;
        item.set_parse_state(PARSE_MULTI_DISPOSITION);
        items->push_back(item);
    }
    if (s == "Content-Type") {
        if (items->empty())
        {
            printf("items is empty! len:%lu\n", length); 
        }
        FileItem &item = (*items)[items->size() - 1];
        item.set_parse_state(PARSE_MULTI_CONTENT_TYPE);
    }
    printf("get multipart_name:%s\n", s.c_str());
    return 0;
}

int cli_parse_disposition_value(const std::string &input, std::string &output) {
    std::vector<std::string> name_tokens;
    cli_split_str(input, '=', name_tokens);
    if (name_tokens.size() != 2) {
        printf("cli_parse_multi_disposition_name(in name) err, input:%s\n", input.c_str());
        return -1;
    } 
    output = name_tokens[1];
    if (output == "\"\"") {
        printf("filename is empty, input:%s\n", input.c_str());
        output = "";
        return 0;
    }
    output = output.substr(1, output.size() - 2); // remove ""
    if (output.empty()) {
        printf("cli_parse_multi_disposition_name(in name) err, name is empty\n");
        return -1;
    }
    return 0;
}

// parse data like "form-data; name="files"; filename="test_thread_cancel.cc""
int cli_parse_multi_disposition(const std::string &input, FileItem &item) {
    std::vector<std::string> pos_tokens;
    cli_split_str(input, ';', pos_tokens);
    if (pos_tokens.size() < 2) {
        printf("cli_parse_multi_disposition_name err, input:%s\n", input.c_str());
        return -1;
    }
    std::string fieldname;
    int ret = cli_parse_disposition_value(pos_tokens[1], fieldname); 
    item.set_name(fieldname);
    if (pos_tokens.size() >=3) {
        item.set_is_file();
        std::string filename;
        ret = cli_parse_disposition_value(pos_tokens[2], filename); 
        item.set_filename(filename);
    }
    return 0;
}

int cli_on_multipart_value(multipart_parser* p, const char *at, size_t length) {
    std::string s;
    s.assign(at, length);
    Response *rsp = (Response *)multipart_parser_get_data(p);
    std::vector<FileItem> *items = rsp->get_file_items();
    if (items->empty()) {
        printf("items is empty in parse multi value!\n");
        return -1;
    }
    FileItem &item = (*items)[items->size() - 1];
    if (item.get_parse_state() == PARSE_MULTI_DISPOSITION) {
        int ret = cli_parse_multi_disposition(s, item);
    }
    if (item.get_parse_state() == PARSE_MULTI_CONTENT_TYPE) {
        item.set_content_type(at, length);
    }
    printf("get multipart_value:%s\n", s.c_str());
    return 0;
}

int cli_on_multipart_data(multipart_parser* p, const char *at, size_t length) {
    if (length == 0) {
        printf("multipart data is empty, len:%lu\n", length);
        return 0;
    }
    Response *rsp = (Response *)multipart_parser_get_data(p);
    std::vector<FileItem> *items = rsp->get_file_items();
    if (items->empty())
    {
        printf("items is empty!, length:%lu\n", length);
    }
    FileItem &item = (*items)[items->size() - 1];
    item.append_data(at, length);
    printf("on multipart data for name:%s, len:%lu\n", 
        item.get_fieldname()->c_str(), length);
    return 0;
}

int cli_on_multipart_data_over(multipart_parser* p) {
    Response *rsp = (Response *)multipart_parser_get_data(p);
    std::vector<FileItem> *items = rsp->get_file_items();
    if(items->empty())
    {
        printf("items is empty!, items size:%lu\n", items->size());
    }
    FileItem &item = (*items)[items->size() - 1];
    item.set_parse_state(PARSE_MULTI_OVER); 
    return 0;
}

int cli_on_multipart_body_end(multipart_parser* p) {
    //Response *req = (Response *)multipart_parser_get_data(p);
    //printf("get multipart_body end, params size:%lu\n", req->get_file_items()->size());
    return 0;//req->parse_multi_params();
}

// parse multipart data like "Content-Type:multipart/form-data; boundary=----GKCBY7qhFd3TrwA"
int cli_parse_multipart_data(Response *rsp) {
    std::string *body = &rsp->get_body();
    if (body->empty()) {
        printf("multipart data is empty\n");
        return 0;
    }
    // parse boundary
    std::string ct = rsp->get_head("Content-Type");
    std::vector<std::string> ct_tokens;
    int ret = cli_split_str(ct, ';', ct_tokens);
    if (ret != 0 || ct_tokens.size() != 2) {
        printf("parse multipart data content type err:%d, ct:%s\n", ret, ct.c_str());
        return ret;
    }
    std::vector<std::string> boundary_tokens;
    ret = cli_split_str(ct_tokens[1], '=', boundary_tokens);
    if (ret != 0 || boundary_tokens.size() != 2) {
        printf("parse multipart data(boundary) content type err:%d, ct:%s\n", ret, ct.c_str());
        return ret;
    }
    std::string boundary = "--" + boundary_tokens[1];
    printf("get bounday:%s\n", boundary.c_str());

    multipart_parser_settings mp_settings;
    memset(&mp_settings, 0, sizeof(multipart_parser_settings));
    mp_settings.on_header_field = cli_on_multipart_name;
    mp_settings.on_header_value = cli_on_multipart_value;
    mp_settings.on_part_data = cli_on_multipart_data;
    mp_settings.on_part_data_end = cli_on_multipart_data_over;
    mp_settings.on_body_end = cli_on_multipart_body_end;

    multipart_parser* parser = multipart_parser_init(boundary.c_str(), &mp_settings);
    multipart_parser_set_data(parser, rsp);
    size_t parsed = multipart_parser_execute(parser, body->c_str(), body->size());
    if (parsed != body->size()) {
        printf("parse multipart data err, parsed:%lu, total:%lu\n", 
            parsed, body->size());
    }
    printf("multipart_parser_execute, parsed:%lu, total:%lu\n", 
        parsed, body->size());
    multipart_parser_free(parser);

    return 0;
}
//http协议转换-http_parser部分
int cli_on_header_field(http_parser *p, const char *buf, size_t len) {
	std::string val(buf,len);
	printf("cli_on_header_field state:%d,val:%s\n", p->state,val.c_str());

	HttpClientData* pClient = (HttpClientData*)p->data;
	Response *rsp = pClient->response;
	if (pClient->lastField.empty())
	{
		printf("httpversion:%d.%d\n",p->http_major, p->http_minor);
		printf("status:%d\n",p->status_code);
		char szVer[20] = {0};
		snprintf(szVer, sizeof(szVer) - 1, "HTTP/%d.%d", p->http_major, p->http_minor);
		rsp->set_head("http_version",szVer);
		rsp->_code_msg.status_code = p->status_code;
	}
	pClient->lastField.assign(buf,len);
	return 0;
}

int cli_on_header_value(http_parser *p, const char *buf, size_t len) {
	HttpClientData* pClient = (HttpClientData*)p->data;
	Response *rsp = pClient->response;
	std::string val(buf,len);
	//printf("cli_on_header_value state:%d,val:%s\n", p->state,val.c_str());
	rsp->set_head(pClient->lastField, std::string(buf,len));
	return 0;
}

int cli_on_headers_complete(http_parser *p) {
	HttpClientData* pClient = (HttpClientData*)p->data;
	Response *rsp = pClient->response;
	std::string ct_header = rsp->get_head("Content-Type");
	printf("content type :[%s]\n",ct_header.c_str());
	return 0;
}

int cli_on_body(http_parser *p, const char *buf, size_t len) {
	HttpClientData* pClient = (HttpClientData*)p->data;
	Response *rsp = pClient->response;
	rsp->get_body().append(buf, len);
	return 0;
}

int cli_on_message_complete(http_parser *p) {
	//设置一个标志位，触发此函数就置为true;
    HttpClientData* pClient = (HttpClientData*)p->data;
    Response *rsp = pClient->response;
    std::string ct_header = rsp->get_head("Content-Type");
//     // parse body params
//     if (ct_header == "application/x-www-form-urlencoded") {
//         std::string *raw_str = req->get_body()->get_raw_string();
//         if (raw_str->size()) {
//             req->get_body()->get_req_params()->parse_query_url(*raw_str);
//         }
//     }
	printf("content type :[%s]\n",ct_header.c_str());
    if (ct_header.find("multipart/form-data") != std::string::npos || ct_header.find("image/jpeg") != std::string::npos) {
       // printf("start parse multipart data! content type:%s\n", ct_header.c_str());
        cli_parse_multipart_data(rsp);
    }
	g_bIsCompelete = true;
	printf("msg COMPLETE!\n");
	return 0;
}


void SetDefaultHeader(std::map<std::string,std::string> header)
{

}

Response& HttpRequestSync(/*OUT*/ Response& response, const std:: string &content, const char* url,emMethod method,int nTimeout,std::map<std::string,std::string> header)
{
	g_bIsCompelete = false;
	char myurl[SS_READ_BUFFER_SIZE], host[CLI_HOST_BUFFER_SIZE], 
		port[CLI_PORT_BUFFER_SIZE], uri[SS_READ_BUFFER_SIZE],
		text[SS_READ_BUFFER_SIZE], *phost = 0;
	char* request = NULL;
	bool bIsContent = false;
	int nReqLen = SS_READ_BUFFER_SIZE;
	int socketid, connectid, reslen, recvid, flag = 1;
	time_t timep;
	time (&timep);
	long letime = timep +  nTimeout/1000;
	hostent *purl = NULL;
	sockaddr_in sockinfo;

	memset(myurl, 0, SS_READ_BUFFER_SIZE);
	memset(host, 0, CLI_HOST_BUFFER_SIZE);
    memset(port, 0, CLI_PORT_BUFFER_SIZE);
	memset(uri, 0, SS_READ_BUFFER_SIZE);
    //设置默认端口为80
    strcpy(port,"80");
	if ((E_POST_METHOD == method) || (E_PUT_METHOD == method))
	{
		bIsContent = true;
		nReqLen = SS_READ_BUFFER_SIZE + content.length();
	}
	request = new char[nReqLen];
	if (NULL == request)
	{
		response._code_msg.status_code = 503;
		response._code_msg.msg = "alloc request memory failed";
		return response;
	}
	memset(request, 0, nReqLen);
	//跳过http或https前缀
	if(0 == strncmp(url,"http",4))
	{
		// 得到:的位置
		for (phost = (char*)url; *phost != ':'; ++phost);
		strcpy(myurl, phost + 3);
	}
	else
	{
		strcpy(myurl, url);
	}

	// 得到path的位置
	for (phost = myurl; *phost != '/' && *phost != '\0'; ++phost);
	if ((int) (phost - myurl) == strlen(myurl)) {
		// 路径为根
		strcpy(uri, "/");
	} else {
		// 将访问路径保存到uri
		strcpy(uri, phost);
	}
	// 将path开始的位置设置为字符串结束标识，myurl到phost即为host
	*phost = '\0';
	// 得到端口的位置
	for (phost = myurl; *phost != ':' &&*phost != '\0'; ++phost);
	if ((int) (phost - myurl) != strlen(myurl)) {
        strcpy(port,phost + 1);
		*phost = '\0';   
	}
	strcpy(host, myurl);
	printf("host:%s,port:%s,uri:%s\n", host,port,uri);
    
	//初始化WSA  必须要加这几行代码
#ifdef _MSC_VER
	WORD sockVersion = MAKEWORD(2,2);
	WSADATA wsaData;
	if(WSAStartup(sockVersion, &wsaData)!=0)
	{
		printf("WSAStartup 失败\n");
	}
#endif
    addrinfo hints;
    addrinfo *result, *rp;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* Any protocol */
    int s = getaddrinfo(host, port, &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        printf("获取服务器地址失败\n");
        response._code_msg.status_code = 503;
        response._code_msg.msg = "get server address failed";
        delete[] request;
        return response;
    }

	///////////////////////////////////////
#ifdef _MSC_VER
#else
	//检查是否会连接阻塞
	int sock = socket(AF_INET,SOCK_STREAM,0);
	//printf("socket-%d\n", sock);	
	if(sock < 0)	
	{		
		perror("sock");		
		response._code_msg.status_code = 503;
		response._code_msg.msg = "get server address failed";
		delete[] request;
		return response;	
	} 	
	int nflag,old_flag;	
	nflag |= O_NONBLOCK;	
	old_flag = nflag = fcntl(sock, F_SETFL, O_NONBLOCK ); //将连接套接字设置为非阻塞。
	rp = result;
	int ret = connect(sock, rp->ai_addr, rp->ai_addrlen);	
	//printf("connect-%d\n", ret);	
	if(ret != 0)	
	{		
		if(errno != EINPROGRESS) //connect返回错误。		
		{			
			printf("connect failed\n");		
			response._code_msg.status_code = 503;
			response._code_msg.msg = "connect failed";
			delete[] request;
			return response;
		}		//连接正在建立		
		else		
		{			
			struct timeval tm;  //1.定时			
			tm.tv_sec = 2;			
			tm.tv_usec = 0; 			
			fd_set wset;			
			FD_ZERO(&wset);			
			FD_SET(sock,&wset); 			
			//printf("selcet start\n");			
			int res = select(sock+1, NULL, &wset, NULL, &tm);			
			//printf("select end\n");			
			if(res <= 0)			
			{				
				printf("select res <= %d\n", res);				
				close(sock);				
				response._code_msg.status_code = 503;
				response._code_msg.msg = "select failed";
				delete[] request;
				return response;			
			}						
			if(FD_ISSET(sock,&wset))			
			{
				printf("test \n");				
				int err = -1;				
				socklen_t len = sizeof(int);								
				if(getsockopt(sock, SOL_SOCKET, SO_ERROR, &err, &len ) < 0) //两种错误处理方式				
				{					
					printf("errno :%d %s\n",errno, strerror(errno));					
					close(sock);					
					response._code_msg.status_code = 503;
					response._code_msg.msg = "getsockopt failed";
					delete[] request;
					return response;				
				} 				
				if(err)				
				{					
					printf("connect faile\n");					
					errno = err;					
					close(sock);					
					response._code_msg.status_code = 503;
					response._code_msg.msg = "get server address failed";
					delete[] request;
					return response;				
				} 				
				printf("connetct success\n");			
			} 		
		} 	
	}
	close(sock);
#endif
	//////////////////////////////////////

    /* getaddrinfo() returns a list of address structures.
        Try each address until we successfully connect(2).
        If socket(2) (or connect(2)) fails, we (close the socket
        and) try the next address. */
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        socketid = socket(rp->ai_family, rp->ai_socktype,
                    rp->ai_protocol);
        if (socketid == -1)
            continue;
        connectid = connect(socketid, rp->ai_addr, rp->ai_addrlen);
        if (connectid != -1)
            break;                  /* Success */
#ifdef _MSC_VER
        closesocket(socketid);
		WSACleanup( );
#else
        close(socketid);
#endif
    }
    if (rp == NULL) {               /* No address succeeded */
        fprintf(stderr, "Could not connect\n");
        printf("连接远端服务器失败\n");
        response._code_msg.status_code = 503;
        response._code_msg.msg = "connect server failed";
        delete[] request;
        return response;
    }
    freeaddrinfo(result);           /* No longer needed */
    printf("-> 连接到远端服务器成功\n");

	// 构造http请求
	std::string strMethod = "";
	switch(method)
	{
	case E_GET_METHOD:
		strMethod = "GET";
		break;
	case E_POST_METHOD:
		strMethod = "POST";
		break;
	case E_PUT_METHOD:
		strMethod = "PUT";
		break;
	case E_DELETE_METHOD:
		strMethod = "DELETE";
		break;
	default:
		printf("error method\n");
	}

	strcat(request, strMethod.c_str());
	strcat(request, " ");
	strcat(request, uri);
	strcat(request, " HTTP/1.1\r\n");
	// 以上为http请求行信息
	strcat(request, "HOST: ");
	strcat(request, host);
	if (0 != strcmp("80",port))
	{
		strcat(request, ":");
		strcat(request, port);
	}
	strcat(request, "\r\n");

	//设置请求头
	if (header.empty())//使用默认请求头
	{
		
		strcat(request, "User-Agent: ");
		strcat(request, "http client");
		strcat(request, "\r\n");
		strcat(request, "Author: ");
		strcat(request, "By ismp");
		strcat(request, "\r\n");
		if (bIsContent)
		{
			//POST的数据格式
			char szContentLen[11] = {0};
			sprintf(szContentLen, "%d", content.length());
			strcat(request, "Content-Type: ");
			strcat(request, "application/json");
			strcat(request, "\r\n");
			strcat(request, "Content-Length: ");
			strcat(request, szContentLen);
			strcat(request, "\r\n");
		}
		strcat(request,"Cache-Control: no-cache\r\n");
		strcat(request,"\r\n");
	}
	else//自定义请求头
	{
		std::map<std::string,std::string>::iterator itr;
		for (itr = header.begin();itr != header.end();++itr)
		{
			strcat(request, itr->first.c_str());
			strcat(request, itr->second.c_str());
			strcat(request, "\r\n");
		}
		strcat(request,"\r\n");
	}
	//正文部分
	if (bIsContent)
	{
		strcat(request,content.c_str());
	}
	// 向服务器发送GET请求
	reslen = send(socketid, request, strlen(request), 0);
	if (reslen == -1) {
		printf("向服务器发送请求失败\n");
		response._code_msg.status_code = 400;
		response._code_msg.msg = "request failed";
		delete[] request;
		return response;
	}
	printf("-> 发送请求成功，共发送了%d bytes\n", reslen);
	printf("-> HTTP请求报文如下\n--------HTTP Request--------\n%s\n", request);

	// 接受服务器的响应
	http_parser_settings _settings;
	http_parser _parser;
	http_parser_settings_init(&_settings);
	_settings.on_header_field = cli_on_header_field;
	_settings.on_header_value = cli_on_header_value;
	_settings.on_headers_complete = cli_on_headers_complete;
	_settings.on_body = cli_on_body;
	_settings.on_message_complete = cli_on_message_complete;

	http_parser_init(&_parser, HTTP_RESPONSE);
	HttpClientData http_client;
	http_client.response = &response;
	_parser.data = &http_client;
	while (flag) {
		memset(text, 0, SS_READ_BUFFER_SIZE);
		int bufflen = recv(socketid, text, SS_READ_BUFFER_SIZE, 0);
		if (bufflen < 0) {
			printf("接收数据流出错\n");
			//fclose(stdout);
            #ifdef _MSC_VER
			closesocket(socketid);
			WSACleanup( );
            #else
			close(socketid);
            #endif
			response._code_msg.status_code = 400;
			response._code_msg.msg = "request failed";
			delete[] request;
			return response;
		}
		if (bufflen > 0) {
			//printf("recv buffer:%d\n", bufflen);
			size_t nparsed = http_parser_execute(&_parser, &_settings, text, bufflen);
			if (nparsed != bufflen) {
				std::string err_msg = "unkonw";
				if (_parser.http_errno) {
					err_msg = http_errno_description(HTTP_PARSER_ERRNO(&_parser));
				}
				printf("parse request error! msg:%s\n", err_msg.c_str());
				response._code_msg.status_code = 400;
				response._code_msg.msg = "request failed";
				delete[] request;
				return response;
			}
			if ((bufflen != SS_READ_BUFFER_SIZE) && g_bIsCompelete)//触发标志位为TRUE，说明http内容读完，退出

			{
				printf("parse SS_READ_BUFFER_SIZE\n");
				flag = 0;
			}
		} else {
			flag = 0;
		}
		time_t timep;
		time (&timep);
		if (timep > letime)
		{
			printf(" outtime %d>%d+%d\n", timep, letime, nTimeout);
			response._code_msg.status_code = 400;
			response._code_msg.msg = "request timeout";
			delete[] request;
			return response;
		}
	}

	#ifdef _MSC_VER
	closesocket(socketid);
	WSACleanup( );
    #else
	close(socketid);
    #endif

	delete[] request;
	return response;
}