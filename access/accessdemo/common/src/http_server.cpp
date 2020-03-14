#include <utility>
#include "http_server.h"

struct mg_serve_http_opts HttpServer::s_server_option;
std::unordered_map<std::string, ReqHandler> HttpServer::s_handler_map;
std::string HttpServer::s_web_dir;

void HttpServer::Init(const std::string &port)
{
    m_port = port;
    s_server_option.enable_directory_listing = "yes";
    s_server_option.document_root = s_web_dir.c_str();
    // TODO����������
}

bool HttpServer::Start()
{
    mg_mgr_init(&m_mgr, NULL);
    mg_connection *connection = mg_bind(&m_mgr, m_port.c_str(), OnHttpEvent);
    if (connection == NULL)
        return false;
    mg_set_protocol_http_websocket(connection);

    printf("starting http server at port: %s\n", m_port.c_str());
    // loop
    while (true)
        mg_mgr_poll(&m_mgr, 500); // ms

    return true;
}

void HttpServer::OnHttpEvent(mg_connection *connection, int event_type, void *event_data)
{
    http_message *http_req = (http_message *)event_data;
    switch (event_type)
    {
    case MG_EV_HTTP_REQUEST:
        HandleEvent(connection, http_req);
        break;
    default:
        break;
    }
}

static bool route_check(http_message *http_msg, char *route_prefix)
{
    if (mg_vcmp(&http_msg->uri, route_prefix) == 0)
        return true;
    else
        return false;

    // TODO: �������ж� GET, POST, PUT, DELTE�ȷ���
    //mg_vcmp(&http_msg->method, "GET");
    //mg_vcmp(&http_msg->method, "POST");
    //mg_vcmp(&http_msg->method, "PUT");
    //mg_vcmp(&http_msg->method, "DELETE");
}

void HttpServer::AddHandler(const std::string &url, ReqHandler req_handler)
{
    if (s_handler_map.find(url) != s_handler_map.end())
        return;

    s_handler_map.insert(std::make_pair(url, req_handler));

    printf("HttpServer::AddHandler url: %s handler: %p\n", url.c_str(), req_handler);

}

void HttpServer::RemoveHandler(const std::string &url)
{
    auto it = s_handler_map.find(url);
    if (it != s_handler_map.end())
        s_handler_map.erase(it);
}

void HttpServer::SendRsp(mg_connection *connection, std::string rsp)
{
    // �����ȷ���header
    mg_printf(connection, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
    // ��json��ʽ����
    mg_printf_http_chunk(connection, "{ \"result\": %s }", rsp.c_str());
    // ���Ϳհ��ַ��죬������ǰ��Ӧ
    mg_send_http_chunk(connection, "", 0);
}

void HttpServer::HandleEvent(mg_connection *connection, http_message *http_req)
{
    std::string req_str = std::string(http_req->message.p, http_req->message.len);
    printf("got request: %s\n", req_str.c_str());

    // �ȹ����Ƿ���ע��ĺ����ص�
    std::string url = std::string(http_req->uri.p, http_req->uri.len);
    std::string body = std::string(http_req->body.p, http_req->body.len);
    auto it = s_handler_map.find(url);

    printf("HttpServer::AddHandler url: %s it: %d\n", url.c_str(), it);

    if (it != s_handler_map.end())
    {
        ReqHandler handle_func = it->second;
        handle_func(url, body, connection, SendRsp);
    }
      // ֱ�ӻش� 200
      SendRsp(connection, "0");

}

bool HttpServer::Close()
{
    mg_mgr_free(&m_mgr);
    return true;
}