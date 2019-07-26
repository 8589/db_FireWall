#ifndef _UI_COMM_H_
#define _UI_COMM_H_

#include "../simple_comm/simple_comm.h"


extern atomic<bool> is_learning;

class ui_comm
{
private:
	int client_fd;
	simple_comm server;
	int db_port;
	logger log;
	char buff[BUFFSIZE];
public:
	ui_comm(int _client, int _db_port):client_fd(_client),db_port(_db_port){};

	void handle_ui_connection();

	void handle_packet(string& msg);

	void switch_mode(string& msg);

	void insert_to_white_list(string& msg);

	int recv_a_packet(string& recv_msg, int _socket_);

	void send_result(char result);

	
};

#endif