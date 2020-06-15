/*************************************************************************
  > File Name: main.cpp
  > Author: amoscykl
  > Mail: amoscykl@163.com 
  > Created Time: Sat 06 Jun 2020 02:27:48 AM PDT
 ************************************************************************/

#include<iostream>
#include "WebsocketServer.h"
using namespace std;


void on_open_handle(server* s,websocketpp::connection_hdl hdl)
{
	printf("on_open!\n");

}
void on_close_handle(server* s,websocketpp::connection_hdl hdl)
{printf("on_close!\n");}
void on_fail_handle(server* s, websocketpp::connection_hdl hdl)
{printf("on_fail!\n");}
void on_message_handle(server* s, websocketpp::connection_hdl hdl, message_ptr msg)
{

	server::connection_ptr con = s->get_con_from_hdl(hdl);
	std::string ip_add = con->get_remote_endpoint();
	websocketpp::http::parser::request m_request = con->get_request();


	// User Agent
	std::string ua = m_request.get_header("User-Agent");
	std::string res = con->get_uri()->get_resource() ;

	printf("ip----------------------->%s\n",ip_add.c_str());
	printf("agent----------------------->%s\n",ua.c_str());
	printf("res----------------------->%s\n",res.c_str());

s->send(hdl, msg->get_payload(), msg->get_opcode());

}




int main(int argc,char *argv[])
{
	websocket_server_callbakc_t cb_arg = {NULL,NULL,NULL,NULL};
	cb_arg.on_open = on_open_handle;
	cb_arg.on_close = on_close_handle;
	cb_arg.on_fail = on_fail_handle;
	cb_arg.on_message = on_message_handle;

	WebsocketServer my_server;
	my_server.init(cb_arg,4567,100);
	my_server.run();
	return 0;
}
