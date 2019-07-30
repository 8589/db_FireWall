#include "db_comm.h"
#include <cstring>


//Summary: handle a connection from a db client

//Parameters:

//Return : 
void db_comm::handle_db_connection()
{
	server.create_socket(AF_INET, SOCK_STREAM, 0);
	server.set_server(AF_INET,"127.0.0.1",this->db_port);
	this->login();
	while(1){
		if(!this->one_comm())
			break;
	}
}


//Summary: imitate the login

//Parameters:

//Return : 
void db_comm::login()
{
	//to server
	server.connect_to_server();

	//from server
	int _size = server.recv_msg();
	server.read_msg(this->buff);

	this->eof_num = ((*(int*)(this->buff+16))&(0x00ffff00) & 0x00000400)?1:2;
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
	server.send_msg(this->client_fd, this->buff, _size);

	
	//from client
	_size = server.recv_msg(this->client_fd);
	server.read_msg(this->buff);
	//log.debug("@@@@@@@@@@@@@user@@@@@@@@@@@@@@@@@@@@\n");
	this->user = string(this->buff+36);
	log.high_debug(string("user: ") + this->user);
	//log.debug("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");

	//this->eof_num = ((*(int*)(this->buff))&(0x0000ffff) & 0x00000200)?1:2;


	//get the count of the eof packet in the result packet
	if(this->eof_num == 1)
		this->eof_num = (((*(int*)(this->buff))&(0x0000ffff) & 0x00000004)?1:2);

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
	server.send_msg(this->buff, _size);

	//from server
	_size = server.recv_msg();
	server.read_msg(this->buff);

	//to client
	server.send_msg(this->client_fd, this->buff, _size);
	


	log.high_debug("login successfully\n");

}


//Summary: finish a complete communication that send query sql from db client and receive query result from db server

//Parameters:

//Return : if the communication is finish,return 1. others such as close the connection or a illegal query will return 0
int db_comm::one_comm()
{
	
	return  this->client_to_server() && this->server_to_client();
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
	int packet_size = this->recv_a_packet(recv_msg, client_fd);

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
	server.send_msg(recv_msg.c_str(), recv_msg.size());
	return 1;
}

//Summary: get all result packet from db server and send them to client

//Parameters:

//Return : the size of all packet from db server to db client
int db_comm::server_to_client()
{
	//from server
	string recv_msg;
	int packet_size = this->recv_a_packet(recv_msg, server.get_socket());
	int all_packet_size = packet_size;
	string all_recv_msg = recv_msg;
	if(recv_msg.size() > 4 && recv_msg[4] != (char)0x00 && recv_msg[4] != (char)0xff && recv_msg[4] != (char)0xfe)
	{
		int eof_cnt = this->eof_num;
		while(eof_cnt)
		{
			packet_size = this->recv_a_packet(recv_msg, server.get_socket());
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
	server.send_msg(this->client_fd, all_recv_msg.c_str(), all_recv_msg.size());
	log.high_debug("operate successflly\n");
	return all_packet_size;
}



//Summary: receive a complete packet

//Parameters:

//       recv_msg:a reference of string type to store the content of packet
//       _socket_ : the socket you want to recerve a packet

//Return : real msg size, not the size of recv_msg
int db_comm::recv_a_packet(string& recv_msg, int _socket_)
{
	recv_msg.clear();
	//get the size of this packet
	int _size = server.recv_msg(_socket_, 4);
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
		_size = server.recv_msg(_socket_, min((unsigned int)BUFFSIZE, total_msg_size-_recv_size));
		server.read_msg(this->buff, _size);
		recv_msg += string(this->buff,this->buff+_size);
		_recv_size += _size;
	}
	return msg_size;
}