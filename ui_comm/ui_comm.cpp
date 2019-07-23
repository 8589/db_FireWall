#include "ui_comm.h"





void ui_comm::handle_ui_connection()
{
	string msg;
	this->recv_a_packet(msg, this->client_fd);
	if(msg.size()>=5)
	{
		//switch mode
		if(msg[4] == '\1')
		{
			printf("switch mode\n");
		}
		//insert a rule
		else if(msg[4] == '\2')
		{
			printf("insert a rule\n");
		}
		//remaining
		else
		{
			printf("nothing\n");
		}
	}
}


int ui_comm::recv_a_packet(string& recv_msg, int _socket_)
{
	recv_msg.clear();
	int _size = server.recv_msg(_socket_);
	if(!_size)	return 0;
	server.read_msg(this->buff);

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