#ifndef _UI_COMM_H_
#define _UI_COMM_H_

#include "simple_comm.h"
#include "connector.h"


extern atomic<bool> is_learning;

class ui_comm
{
private:
	int client_fd;
	simple_comm server;
	int db_port;
	logger log;
	char buff[BUFFSIZE];
	connector conn;
public:

	~ui_comm(){
		server.close_socket(this->client_fd);
		server.close_socket();
	}
	ui_comm(int _client, int _db_port):client_fd(_client),db_port(_db_port){};

	void handle_ui_connection();

	int handle_packet(string& msg);

	int switch_mode(string& msg);

	int update_a_rule(string& msg);

	int recv_a_packet(string& recv_msg, int _socket_);

	int send_result(char result);

};

#endif
