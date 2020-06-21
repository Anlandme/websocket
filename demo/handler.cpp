#include<iostream>
#include "WebsocketServer.h"
#include "define.h"
#include "esr.h"

using namespace std;

static std::map<websocketpp::connection_hdl,connection_info_t> connection_info_map;
static std::map<pEsrInstBase,websocketpp::connection_hdl> esr_instmap;



extern EsrInterface* pEsrMgr;

void on_open_handle(server* s,websocketpp::connection_hdl hdl)
{
	int ret;

	pEsrInstBase esrinst;
	connection_info_t info;
	printf("on_open!\n");

	//get client info
	server::connection_ptr con = s->get_con_from_hdl(hdl);
	std::string ip_add = con->get_remote_endpoint();
	websocketpp::http::parser::request m_request = con->get_request();

	std::string ua = m_request.get_header("User-Agent");
	std::string res = con->get_uri()->get_resource();
#if 0
	printf("ip----------------------->%s\n",ip_add.c_str());
	printf("agent----------------------->%s\n",ua.c_str());
	printf("res----------------------->%s\n",res.c_str());
#endif


	//create esr engine
	ret = create_esr(&esrinst);
	if(0!=ret)
	{
		printf("esr create failed!\n");
		return;
	}

	memset(&info,0,sizeof(info));
	memcpy(&info.ip,ip_add.c_str(),strlen(ip_add.c_str()));
	memcpy(&info.res,res.c_str(),strlen(res.c_str()));
	info.esr_inst = esrinst;

	connection_info_map.insert(make_pair(hdl,info));

	//start esr
	ret = start_esr(esrinst);
	if(0!=ret)
	{
		printf("esr start failed!\n");
		return;
	}

	//must!tell the engine start do esr
	char temp[1024] = {0};
	write_esr(esrinst,temp,1024,ESR_AUDIO_BEGIN);

}
void on_close_handle(server* s,websocketpp::connection_hdl hdl)
{
	printf("on_close!\n");
	connection_info_map.erase(hdl);
}
void on_fail_handle(server* s, websocketpp::connection_hdl hdl)
{
	printf("on_fail!\n");
}
void on_message_handle(server* s, websocketpp::connection_hdl hdl, message_ptr msg)
{
	int ret = 0;
	std::string result;

	//get esr inst
	std::map<websocketpp::connection_hdl,connection_info_t>::iterator it;
	it =  connection_info_map.find(hdl);

#if 0	
	printf("ip----------------------->%s\n",it->second.ip);
	printf("res----------------------->%s\n",it->second.res);
#endif

	ret = write_esr(it->second.esr_inst,msg->get_payload().c_str(),msg->get_payload().size(),ESR_AUDIO_CONTINUE);
	//printf("write count:%d\n",ret):



	ret = read_esr(it->second.esr_inst,result);

#if 0
	std::remove(result.begin(), result.end(), '\r');
	std::remove(result.begin(), result.end(), '\n');
	std::remove(result.begin(), result.end(), '<');
	std::remove(result.begin(), result.end(), '>');
#endif

	if(0 == ret)
	{
		return;
	}

	if(result.size()>0)
	{
#if 0
		std::remove(result.begin(), result.end(), '\r');
		std::remove(result.begin(), result.end(), '\n');
		std::remove(result.begin(), result.end(), '<');
		std::remove(result.begin(), result.end(), '>');
		std::remove(result.begin(), result.end(), 's');
		std::remove(result.begin(), result.end(), '/');
#endif
		s->send(hdl,result.c_str(),websocketpp::frame::opcode::binary);
//		printf("result:%s\n",result.c_str());

		result.clear();

		//the engine is stop
		if(2 == ret)
		{
//			printf("--------------------->\n");
			ret = stop_esr(it->second.esr_inst);
			ret = start_esr(it->second.esr_inst);

			if(0!=ret)
			{
				printf("esr start failed!\n");
				return;
			}

			//must!tell the engine start do esr
			char temp[1024] = {0};
			write_esr(it->second.esr_inst,temp,1024,ESR_AUDIO_BEGIN);
		}
	}

}

void do_sm()
{
	ESRInit();
#if 1
	websocket_server_callbakc_t cb_arg = {NULL,NULL,NULL,NULL};
	cb_arg.on_open = on_open_handle;
	cb_arg.on_close = on_close_handle;
	cb_arg.on_fail = on_fail_handle;
	cb_arg.on_message = on_message_handle;

	WebsocketServer my_server;
	my_server.init(cb_arg,4567,100);
	my_server.run();
#endif
}




