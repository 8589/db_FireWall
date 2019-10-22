#include "simple_comm.h"

int main(){
	simple_comm client;
	client.create_socket(AF_INET, SOCK_STREAM, 0);
	client.set_server(AF_INET,"127.0.0.1",6666);
	client.connect_to_server();
	char buff[BUFFSIZE];
	client.recv_msg();
	client.read_msg(buff);
	printf("receive msg: %s\n", buff);
	client.send_msg("???");
	//sleep(3000);
	client.close_socket();
}