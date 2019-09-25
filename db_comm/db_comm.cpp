#include "db_comm.h"
#include <cstring>


int db_comm::handle_db_connection()
{
	if(server.create_socket(AF_INET, SOCK_STREAM, 0) < 0)
		return -1;
	server.set_server(AF_INET,"127.0.0.1",this->db_port);
	if(this->login() < 0)
		return -1;
	while(1){
		int res = this->one_comm();
		if(res < 0)
			return -1;
		if(!res)
			break;
	}
	return 0;
}


int db_comm::login()
{
	//to server
	if ( server.connect_to_server() < 0)
		return -1;

	//from server
	int _size = 0;
	if((_size = server.recv_msg()) < 0)
		return -1;
	server.read_msg(this->buff);

	//this->eof_num = ((*(int*)(this->buff+16))&(0x00ffff00) & 0x00000400)?1:2;
	//cout << "eof_num=" << this->eof_num << endl;
	/*
	this->eof_num = (*(int*)(this->buff+16))&(0x03ffff00);
	printf("eof_num: %d\n", this->eof_num);
	for(int i=0x00000100;i<0x04000000;i=i<<1)
	{
		if(this->eof_num & i)
			printf("1\n");
		else 
			printf("0\n");
	}
	*/

	//to client
	if(server.send_msg(this->client_fd, this->buff, _size) < 0)
		return -1;

	
	//from client
	if((_size = server.recv_msg(this->client_fd)) < 0)
		return -1;
	
	server.read_msg(this->buff);
	//log.debug("@@@@@@@@@@@@@user@@@@@@@@@@@@@@@@@@@@\n");
	this->user = string(this->buff+36);
	log.high_debug(string("user: ") + this->user);
	//log.debug("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");

	//this->eof_num = ((*(int*)(this->buff))&(0x0000ffff) & 0x00000200)?1:2;


	//get the count of the eof packet in the result packet
	//if(this->eof_num == 1)
	//	this->eof_num = (((*(int*)(this->buff))&(0x0000ffff) & 0x00000004)?1:2);
	//cout << "eof_num=" << this->eof_num << endl;
	this->eof_num = 1;
	int *ip = (int*)this->buff;
	for(int i=0;i<32;i++){
		if(*ip & (1<<i))
			printf("1");
		else
			printf("0");
	}
	printf("\n");
	/*
	printf("eof_num: %d\n", this->eof_num);
	for(int i=0x00000001;i<0x00040000;i=i<<1)
	{
		if(this->eof_num & i)
			printf("1\n");
		else 
			printf("0\n");
	}
	*/

	//to server

	if(server.send_msg(this->buff, _size) < 0)
		return -1;

	//from server
	if((_size = server.recv_msg()) < 0)
		return -1;
	server.read_msg(this->buff);

	//to client

	if(server.send_msg(this->client_fd, this->buff, _size) < 0)
		return -1;
	
	log.high_debug("login successfully\n");
	return 1;

}

int db_comm::one_comm()
{
	int res= this->client_to_server();
	if(res < 0)
		return -1;
	else if(res == 0)
		return 0;

	res = this->server_to_client();
	if(res < 0)
		return -1;
	else if(res == 0)
		return 0;
	return 1;
}


//Summary: how to handle a illegal query. this func can rewrite by the requirement

//Parameters:

//Return : just return 0;
int db_comm::hanlde_illegal_query()
{
	log.warning("a illegal query\n");
	return 0;
}



//Summary: get a query packet from db client and send them to db server

//Parameters:

//Return : if the filter is learning mode or the filter is protect mode and the query sql is legal the func will return 1, others will return 0
int db_comm::client_to_server()
{
	//from client
	string recv_msg;
	string sql;
	int packet_size;
	if((packet_size = this->recv_a_packet(recv_msg, client_fd)) < 0)
		return -1;
	if(packet_size == 0)
		return 0;

	char msg[MSGSIZE];
	sprintf(msg, "the size of the packet receive from client: %lu",recv_msg.size());
	log.high_debug(msg);

	//fix big data msg
	if(recv_msg.size() >= 5){
		sql = string(recv_msg,5,recv_msg.size()-5);
		log.high_debug(sql.c_str());
		//printf("%s\n", sql.c_str());

		//to open filter
		filter f;
		if(this->mode){
			f.add_white_list_n_times(this->user, sql, 4, string(inet_ntoa((this->client_addr).sin_addr)));
			//f.add_white_list((this->user).c_str(), sql.c_str());
		}else{
			if(!f.is_legal_and_add_log((this->user).c_str(), sql.c_str(), string(inet_ntoa((this->client_addr).sin_addr))))
			{
				return this->hanlde_illegal_query();
			}
		}
}
	//to server
	if(server.send_msg(recv_msg.c_str(), recv_msg.size()) < 0)
		return -1;
	return 1;
}

//Summary: get all result packet from db server and send them to client

//Parameters:

//Return : the size of all packet from db server to db client
int db_comm::server_to_client()
{
	//from server
	string recv_msg;
	int packet_size ;
	if((packet_size = this->recv_a_packet(recv_msg, server.get_socket())) < 0)
		return -1;
	if(packet_size == 0)
		return 0;
	int all_packet_size = packet_size;
	string all_recv_msg = recv_msg;
	if(recv_msg.size() > 4 && recv_msg[4] != (char)0x00 && recv_msg[4] != (char)0xff && recv_msg[4] != (char)0xfe)
	{
		int eof_cnt = this->eof_num;
		while(eof_cnt)
		{
			if((packet_size = this->recv_a_packet(recv_msg, server.get_socket())) < 0)
				return -1;
			if(packet_size == 0)
				break;
			all_packet_size += packet_size;
			all_recv_msg += recv_msg;
			if(recv_msg.size() > 4 && recv_msg[4] == (char)0xfe)
				eof_cnt--;
		}
	}
	char msg[MSGSIZE];
	sprintf(msg, "the size of the packet receive from server: %lu", all_recv_msg.size());
	log.high_debug(msg);
	//to client
	if(server.send_msg(this->client_fd, all_recv_msg.c_str(), all_recv_msg.size()) < 0)
		return -1;
	log.high_debug("operate successflly\n");
	//return all_packet_size;
	return 1;
}


int db_comm::recv_a_packet(string& recv_msg, int _socket_)
{
	recv_msg.clear();
	//get the size of this packet
	int _size = server.recv_msg(_socket_, 4);
	if(_size  < 0)
		return -1;

	if(!_size)	return 0;
	
	server.read_msg(this->buff, _size);

	//get the size of real msg 
	unsigned int msg_size = (*((unsigned int*)buff))&(0x00ffffffu);

	//get the size of real packet
	unsigned int total_msg_size = msg_size + 4;
	recv_msg = string(this->buff,this->buff+_size);
	unsigned int _recv_size = _size;

	while(_recv_size < total_msg_size)
	{
		if((_size = server.recv_msg(_socket_, min((unsigned int)BUFFSIZE, total_msg_size-_recv_size))) < 0)
			return -1;
		server.read_msg(this->buff, _size);
		recv_msg += string(this->buff,this->buff+_size);
		_recv_size += _size;
	}
	return msg_size;
}