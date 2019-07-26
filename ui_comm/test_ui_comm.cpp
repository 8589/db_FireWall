#include "ui_comm.h"

int main()
{
	simple_comm server;
	server.create_socket(AF_INET, SOCK_STREAM, 0);
	server.set_server(AF_INET,"0.0.0.0",6667);
	server.bind_socket();
	server.listen_client(10);
	while(1){
		sockaddr_in client_addr;
		int client = server.accept_client(&client_addr);
		thread t([](int _client, int port){
			ui_comm uc(_client, port);
			uc.handle_ui_connection();
		}, client, 3306);
		t.detach();
	}
	server.close_socket();
}