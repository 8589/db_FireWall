#include "simple_comm.h"

int main(){
	simple_comm server;
	server.create_socket(AF_INET, SOCK_STREAM, 0);
	server.set_server(AF_INET,"0.0.0.0",6666);
	server.bind_socket();
	server.listen_client(10);
	while(1){
		sockaddr_in client_addr;
		int client = server.accept_client(&client_addr);
		server.send_msg(client, "hello,client");

		int msg_size = server.recv_msg(client);
		char buff[BUFFSIZE];
		server.read_msg(buff);
		printf("receive msg: %s\n", buff);
		
		server.close_socket(client, &client_addr);
	}
	server.close_socket();
}