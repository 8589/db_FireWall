#ifndef _DB_COMM_H_
#define _DB_COMM_H_

#include "simple_comm.h"
#include "filter.h"
#include <vector>
#include <string>

using namespace std;

class db_comm
{
private:
	simple_comm server;		//handle network communication operation
	int client_fd;			//the fd of the client socket
	int db_port;			//the db server port
	char buff[BUFFSIZE];	//the buff to receive packet
	logger log;				// log
	string user;			// client user
	sockaddr_in client_addr;	//get the ip of client
	bool mode;	//learing or prtection mode
	int eof_num;
	filter f;

public:
	//sql_comm();
	db_comm(int client_fd, int _db_port, sockaddr_in _client_addr, bool _mode):
		client_fd(client_fd),db_port(_db_port),client_addr(_client_addr),mode(_mode){this->eof_num = 2;}
	~db_comm()
	{
		server.close_socket(this->client_fd, &(this->client_addr));
		server.close_socket();
	}

	int handle_db_connection();

	int login();

	int select_one_comm(string& s);
	int check_sql(string& recv_msg);
	int one_comm();

	int hanlde_illegal_query();

	int client_to_server();

	int server_to_client();

	int recv_a_packet(string& recv_msg, int _socket_);

};

#endif
