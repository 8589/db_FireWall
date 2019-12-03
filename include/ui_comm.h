#ifndef _UI_COMM_H_
#define _UI_COMM_H_

#include "simple_comm.h"
#include "connector.h"
#include "naive_filter.h"
#include "CJsonObject.hpp"
#include "naive_sql_parser.h"
#include <string>
#include <fstream>

using namespace std;


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
	shared_ptr<naive_filter> sp;
public:
	ui_comm(){
		sp = shared_ptr<naive_filter>( new naive_filter ( shared_ptr<sql_parser>(new naive_sql_parser) ) );
	}
	ui_comm(int _client, int _db_port):client_fd(_client),db_port(_db_port){
		sp = shared_ptr<naive_filter>( new naive_filter ( shared_ptr<sql_parser>(new naive_sql_parser) ) );
	};

	~ui_comm(){
		server.close_socket(this->client_fd);
		server.close_socket();
	}
	

	void handle_ui_connection();

	int handle_packet(string& msg);

	int switch_mode(string& msg);

	int update_a_rule(string& msg);

	int handle_config(const string& msg);

	string read_config();

	void change_config(const string& config);

	int recv_a_packet(string& recv_msg, int _socket_);

	int send_result(char result);

	int send_result(string result);



};

#endif
