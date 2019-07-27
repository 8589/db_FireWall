#include "firewall.h"



void firewall::start_firewall()
{
	simple_comm server;
	server.create_socket(AF_INET, SOCK_STREAM, 0);
	server.set_server(AF_INET,"0.0.0.0",6666);
	server.bind_socket();
	server.listen_client(10);
	while(1){
		sockaddr_in client_addr;
		int client = server.accept_client(&client_addr);
		printf("is_learning: %d\n", is_learning.load());
		thread t([](int _client, int port, sockaddr_in _client_addr, bool _mode){
			db_comm dc(_client, port, _client_addr, _mode);
			dc.handle_db_connection();
		}, client, 3306, client_addr, is_learning.load());
		t.detach();
	}
	server.close_socket();
}

void firewall::comm_with_web_UI()
{
	simple_comm server;
	server.create_socket(AF_INET, SOCK_STREAM, 0);
	server.set_server(AF_INET,"0.0.0.0",6667);
	server.bind_socket();
	server.listen_client(10);
	while(1){
		sockaddr_in client_addr;
		int client = server.accept_client(&client_addr);
		thread t([](int _client, int port, sockaddr_in _client_addr){
			ui_comm uc(_client, port);
			uc.handle_ui_connection();
		}, client, 3306, client_addr);
		t.detach();
	}
	server.close_socket();
}
