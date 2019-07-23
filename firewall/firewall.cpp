#include "firewall.h"


void firewall::start()
{
	simple_comm server;
	server.create_socket(AF_INET, SOCK_STREAM, 0);
	server.set_server(AF_INET,"0.0.0.0",6666);
	server.bind_socket();
	server.listen_client(10);
	while(1){
		sockaddr_in client_addr;
		int client = server.accept_client(&client_addr);
		thread t([](int _client, int port, sockaddr_in _client_addr, int _mode){
			db_comm dc(_client, port, _client_addr, _mode);
			dc.handle_db_connection();
		}, client, 3306, client_addr, 1);
		t.detach();
	}
	server.close_socket();
}

/*
void firewall::scomm_with_web_UI()
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
			db_comm dc(_client, port, _client_addr);
			dc.handle_db_connection();
		}, client, 3306, client_addr);
		t.detach();
	}
	server.close_socket();
}
*/