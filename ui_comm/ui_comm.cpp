#include "ui_comm.h"





void ui_comm::handle_ui_connection()
{
	string msg;
	this->recv_a_packet(msg, this->client_fd);
	printf("###%lu\n",msg.size());
	this->handle_packet(msg);
}

void ui_comm::handle_packet(string& msg)
{
	if(msg.size()>=5)
	{
		//switch mode
		if(msg[4] == '\1')
		{
			this->switch_mode(msg);
		}
		//insert a rule
		else if(msg[4] == '\2')
		{
			this->update_a_rule(msg);
		}
		//remaining
		else
		{
			printf("nothing\n");
		}
	}
}

void ui_comm::switch_mode(string& msg)
{
	printf("switch mode\n");
	if(msg.size() >= 6)
	{
		if(msg[5] == '\1')
		{
			printf("on\n" );
			is_learning.store(true);
			this->send_result(1);
		}
		else if(msg[5] == '\0')
		{
			printf("off\n");
			is_learning.store(false);
			this->send_result(1);
		}
		else if(msg[5] == '\2')
		{
			printf("query mode\n");
			this->send_result(is_learning.load());
		}
		else if(msg[5] == '\4')
		{
			printf("just switch\n");
			is_learning.store(is_learning.load()^1);
			this->send_result(is_learning.load());
		}
		else
		{
			printf("fff\n");
			this->send_result(1);
		}
	}
	else
	{
		printf("kkk\n");
		this->send_result(0);
	}
}

void ui_comm::update_a_rule(string& msg)
{

	printf("update a rule\n");
	if(msg.size() >= 7)
	{
		int level = msg[5];
		int update = msg[6];
		string user = string(msg, 7, 30);
		string addr_ip = string(msg, 7+30, 30);
		string sql = string(msg, 7+30+30, msg.size()-7-30-30);
		printf("user: %s\n", user.c_str());
		printf("addr_ip: %s\n", addr_ip.c_str());
		printf("level: %d\n", level);
		printf("update: %d\n", update);
		printf("sql: %s\n", sql.c_str());
		(this->conn).connect_to_db();
		if(update && 2)
		{
			(this->conn).insert_to_a_list(user, sql, level, addr_ip, (update&1)+1);
		}
		else
		{
			(this->conn).remove_from_a_list(user, sql, addr_ip, (update&1)+1);
		}
		(this->conn).close();
		this->send_result(1);
		printf("####\n");
	}
	else
	{
		this->send_result(0);
	}
}




int ui_comm::recv_a_packet(string& recv_msg, int _socket_)
{
	recv_msg.clear();
	int _size = server.recv_msg(_socket_, 4);
	if(!_size)	return 0;
	server.read_msg(this->buff, _size);

	unsigned int msg_size = (*((unsigned int*)buff))&(0xffffffffu);

	//unsigned int msg_index = (unsigned char)(*(buff+3));
	//printf("$$%u\n", msg_index);

	unsigned int total_msg_size = msg_size + 4;
	recv_msg = string(this->buff,this->buff+_size);
	unsigned int _recv_size = _size;

	while(_recv_size < total_msg_size)
	{
		_size = server.recv_msg(this->client_fd, min((unsigned int)BUFFSIZE,total_msg_size-_recv_size));
		server.read_msg(this->buff, _size);
		recv_msg += string(this->buff,this->buff+_size);
		_recv_size += _size;
	}
	return msg_size;
}



void ui_comm::send_result(char result)
{
	char res = result;
	server.send_msg(this->client_fd, &res, 1);
}