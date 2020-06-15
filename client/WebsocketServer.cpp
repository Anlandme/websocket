#include "WebsocketServer.h"

WebsocketServer::WebsocketServer()
{
	m_server = new server();
}


WebsocketServer::~WebsocketServer()
{}


int WebsocketServer::init(websocket_server_callbakc_t cb,int listen_port,int max_load_count)
{
	m_server->set_access_channels(websocketpp::log::alevel::none);
	// m_server->clear_access_channels(websocketpp::log::alevel::frame_payload);

	// Initialize Asio
	m_server->init_asio();

	// Register our message handler
	if(NULL != cb.on_open)
	{
		m_server->set_open_handler(bind(cb.on_open,m_server,::_1));
	}

	if(NULL != cb.on_close)
	{
		m_server->set_close_handler(bind(cb.on_close,m_server,::_1));
	}

	if(NULL != cb.on_fail)
	{
		m_server->set_fail_handler(bind(cb.on_fail,m_server,::_1));
	}

	if(NULL != cb.on_message)
	{
		m_server->set_message_handler(bind(cb.on_message,m_server,::_1,::_2));
	}

	// Listen on port
	m_server->listen(listen_port);
}

void WebsocketServer::run()
{
	try
	{
		m_server->start_accept();
		m_server->run();
	} 
	catch (websocketpp::exception const & e) 
	{
		std::cout << e.what() << std::endl;
	} 
	catch (...) 
	{
		std::cout << "other exception" << std::endl;
	}
}
void WebsocketServer::stop()
{}
void WebsocketServer::destory()
{}

void WebsocketServer::sendMsg(char* msg)
{}
