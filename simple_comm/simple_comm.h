#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include "../logger/logger.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>

#define	MAX_MSG_SIZE 16777215
#define BUFFSIZE 4096
#define MSGSIZE 1024
class simple_comm
{
	char buff[BUFFSIZE];
	int _socket;
	logger log;
	
public:
	sockaddr_in addr;

	void create_socket(int domain, int type, int protocol=0);

	void set_server(int domain, string addr, int port);

	void bind_socket();

	void listen_client(int _size);

	void connect_to_server();

	void send_msg(const char* msg, int _size);
	void send_msg(int _socket_, const char* msg, int _size);

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