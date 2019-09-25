#include "firewall.h"



void firewall::start_firewall(int db_server_port, int firewall_port)
{
	simple_comm server;
	if(server.create_socket(AF_INET, SOCK_STREAM, 0) < 0){
		exit(0);
	}
	server.set_server(AF_INET,"0.0.0.0",firewall_port);
	if(server.bind_socket() < 0){
		exit(0);
	}
	if(server.listen_client(10) < 0){
		exit(0);
	}
	while(1){
		sockaddr_in client_addr;
		int client = server.accept_client(&client_addr);
		if(client < 0){
			continue;
		}
		thread t([](int _client, int port, sockaddr_in _client_addr, bool _mode){
			db_comm dc(_client, port, _client_addr, _mode);
			dc.handle_db_connection();
		}, client, db_server_port, client_addr, is_learning.load());
		t.detach();
	}
	server.close_socket();
}

void firewall::comm_with_web_UI(int db_server_port, int ui_comm_port)
{
	simple_comm server;
	server.create_socket(AF_INET, SOCK_STREAM, 0);
	server.set_server(AF_INET,"0.0.0.0", ui_comm_port);
	server.bind_socket();
	server.listen_client(10);
	while(1){
		sockaddr_in client_addr;
		int client = server.accept_client(&client_addr);
		thread t([](int _client, int port, sockaddr_in _client_addr){
			ui_comm uc(_client, port);
			uc.handle_ui_connection();
		}, client, db_server_port, client_addr);
		t.detach();
	}
	server.close_socket();
}
