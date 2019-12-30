#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include "mongoose.h"

//// ����http����callback
//typedef void OnRspCallback(mg_connection *c, std::string);
//// ����http����handler
//using ReqHandler = std::function<bool (std::string, std::string, mg_connection *c, OnRspCallback)>;
//
//class HttpServer
//{
//public:
//    HttpServer() {}
//    ~HttpServer() {}
//    void Init(const std::string &port); // ��ʼ������
//    bool Start(); // ����httpserver
//    bool Close(); // �ر�
//    void AddHandler(const std::string &url, ReqHandler req_handler); // ע���¼�������
//    void RemoveHandler(const std::string &url); // �Ƴ�ʱ�䴦����
//    static std::string s_web_dir; // ��ҳ��Ŀ¼ 
//    static mg_serve_http_opts s_server_option; // web������ѡ��
//    static std::unordered_map<std::string, ReqHandler> s_handler_map; // �ص�����ӳ���
//
//private:
//    // ��̬�¼���Ӧ����
//    static void OnHttpEvent(mg_connection *connection, int event_type, void *event_data);
//    static void HandleEvent(mg_connection *connection, http_message *http_req);
//    static void SendRsp(mg_connection *connection, std::string rsp);
//
//    std::string m_port;    // �˿�
//    mg_mgr m_mgr;          // ���ӹ�����
//};




// ����http����callback
typedef void OnRspCallback(mg_connection *c, std::string);
// ����http����handler
typedef bool (*ReqHandler)(std::string, std::string, mg_connection *c, OnRspCallback);	

//using ReqHandler = std::function<bool (std::string, std::string, mg_connection *c, OnRspCallback)>;

class HttpServer
{
public:
	HttpServer() {}

	~HttpServer() {}
	void Init(const std::string &port); // ��ʼ������
	bool Start(); // ����httpserver
	bool Close(); // �ر�
	void AddHandler(const std::string &url, ReqHandler req_handler); // ע���¼�������
	void RemoveHandler(const std::string &url); // �Ƴ�ʱ�䴦����
	static struct mg_serve_http_opts s_server_option; // web������ѡ��
	static std::unordered_map<std::string, ReqHandler> s_handler_map; // �ص�����ӳ���
	static std::string s_web_dir; // ��ҳ��Ŀ¼ 
public:
	//��̬�¼���Ӧ����
	static void OnHttpEvent(mg_connection *connection, int event_type, void *event_data);
	static void SendRsp(mg_connection *connection, std::string rsp);
	static void HttpEventReq(mg_connection *connection, int event_type, void *event_data);
	static void HandleEvent(mg_connection *connection, http_message *http_req);
	static void http_send_rsp(mg_connection *connection, std::string rsp);
	bool handle_transactions(std::string url, std::string body, mg_connection *c, OnRspCallback rsp_callback);    //added 

private:
	std::string m_port;    // �˿�
	mg_mgr m_mgr;          // ���ӹ�����
};