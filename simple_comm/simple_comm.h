#ifndef _SIMPLE_COMM_H_
#define _SIMPLE_COMM_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include "../logger/logger.h"
#include "../config/config.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>


class simple_comm
{
	char buff[BUFFSIZE];
	int _socket;
	logger log;
	
public:
	sockaddr_in addr;

	int create_socket(int domain, int type, int protocol=0);

	void set_server(int domain, string addr, int port);

	int bind_socket();

	int listen_client(int _size);

	int connect_to_server();

	int send_msg(const char* msg, int _size);
	int send_msg(int _socket_, const char* msg, int _size);

	int recv_msg();
	int recv_msg(int _socket_);
	int recv_msg(int _socket_, int _size);

	void read_msg(char* buf);
	void read_msg(char* buf, int _size);
	

	int accept_client();
	int accept_client(sockaddr_in* client_addr);

	void close_socket();
	void close_socket(int _socket_);
	void close_socket(int _socket_, sockaddr_in* client_addr);

	int get_socket();

};


#endif