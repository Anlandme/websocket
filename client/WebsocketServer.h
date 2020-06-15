#ifndef __WEBSOCKET_SERVER__H__
#define __WEBSOCKET_SERVER__H__

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <string>
#include <sstream>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;


typedef websocketpp::server<websocketpp::config::asio> server;
typedef server::message_ptr message_ptr; 



typedef void (*websocket_server_on_open_handle)(server* s,websocketpp::connection_hdl hdl);
typedef void (*websocket_server_on_close_handle)(server* s,websocketpp::connection_hdl hdl);
typedef void (*websocket_server_on_fail_handle)(server* s, websocketpp::connection_hdl hdl);
typedef void (*websocket_server_on_message_handle)(server* s, websocketpp::connection_hdl hdl, message_ptr msg);


typedef struct webscoket_server_callback_s
{
	websocket_server_on_open_handle on_open;
	websocket_server_on_close_handle on_close;
	websocket_server_on_fail_handle on_fail;
	websocket_server_on_message_handle on_message;
}websocket_server_callbakc_t;

class   WebsocketServer
{
	public:
		WebsocketServer();
		~WebsocketServer();

		int init(websocket_server_callbakc_t cb,int listen_port,int max_load_count);

		void run();
		void stop();
		void destory();

		void sendMsg(char* msg);

	private:
		server* m_server;
};


#endif
