#include "ui_comm.h"


void ui_comm::handle_ui_connection()
{
	string msg;
	if(this->recv_a_packet(msg, this->client_fd) <= 0)
		return;
	//printf("###%lu\n",msg.size());
	this->handle_packet(msg);
}

int ui_comm::handle_packet(string& msg)
{
	if(msg.size()>=5)
	{
		//switch mode
		if(msg[4] == '\1')
		{
			return this->switch_mode(msg);
		}
		//insert a rule
		else if(msg[4] >= '\2' && msg[4] <= '\6')
		{
			return this->update_a_rule(msg);
		}
		//remaining
		else
		{
			log.debug("do nothing!");
		}
	}
	return 1;
}

int ui_comm::switch_mode(string& msg)
{
	log.debug("switch mode");
	if(msg.size() >= 6)
	{
		if(msg[5] == '\1')
		{
			log.debug("on");
			is_learning.store(true);
			if(this->send_result(1) < 0)
				return -1;
		}
		else if(msg[5] == '\0')
		{
			log.debug("off");
			is_learning.store(false);
			if(this->send_result(1) < 0)
				return -1;
		}
		else if(msg[5] == '\2')
		{
			log.debug("query mode");
			if(this->send_result(is_learning.load()) < 0)
				return -1;
		}
		else if(msg[5] == '\4')
		{
			log.debug("just switch");
			is_learning.store(is_learning.load()^1);
			if(this->send_result(is_learning.load()) < 0)
				return -1;
		}
		else
		{
			log.debug("switch mode -> nothing");
			if(this->send_result(1) < 0)
				return -1;
		}
	}
	else
	{
		log.debug("switch mode -> no info for what to do");
		if(this->send_result(0) < -1)
			return -1;
	}
	return 1;
}

int ui_comm::update_a_rule(string& msg)
{
	log.debug("update a rule");
	//printf("%d\n", msg[4]);
	if(msg.size() >= 7)
	{
		int level = msg[5];
		int flag = msg[6];
		string user = string(msg, 7, 30);
		user = string(user.c_str());
		string addr_ip = string(msg, 7+30, 30);
		addr_ip = string(addr_ip.c_str());
		string sql = string(msg, 7+30+30, msg.size()-7-30-30);

		char _msg[MSGSIZE];
		sprintf(_msg, "user: %s\naddr_ip:%s\nlevel: %d\nflag: %d\nsql: %s\n",user.c_str(), addr_ip.c_str(), level,flag,sql.c_str());
		log.high_debug(_msg);
		
		//printf("%d\n", msg[4]);
		//parse
		if(msg[4] == '\2'){
			log.debug("parse");
			vector<string> res;
			//
			sp->parse(sql, res);

			if(this->send_result(res.size()) < 0)
				return -1;
			for(int i=0;i<res.size();i++){
				if(this->send_result(res[i]) < 0)
					return -1;
			}
		}
		//insert
		else if(msg[4] == '\3'){
			log.debug("insert");
			sp->insert_to_db(sql, user, level&(1<<4), addr_ip, level&(1<<3));
		}
		//delete
		else if(msg[4] == '\4'){
			log.debug("delete");
			sp->delete_from_db(sql, user, addr_ip);
		}
		//remove 
		else if(msg[4] == '\5'){
			log.debug("remove");
			sp->remove_from_list(sql, level, user, addr_ip, flag);
		}
		//add
		else if(msg[4] == '\6'){
			log.debug("add");
			sp->add_to_list(sql, level, user, addr_ip, flag);
		}
		else{
			log.debug("ddddddddddddamn");
		}
		if(this->send_result(1) < 0)
			return -1;
	}
	else
	{
		if(this->send_result(0) < 0)
			return -1;
	}
	return 1;
}




int ui_comm::recv_a_packet(string& recv_msg, int _socket_)
{
	recv_msg.clear();
	int _size = server.recv_msg(_socket_, 4);
	if(_size <= 0)
		return _size;

	server.read_msg(this->buff, _size);

	//unsigned int msg_size = (*((unsigned int*)buff))&(0xffffffffu);
	unsigned int msg_size = 0;
	for(int i=3;i>=0;i--){
		msg_size = msg_size*256 + (unsigned char)*(buff+i);
	}

	//unsigned int msg_index = (unsigned char)(*(buff+3));
	//printf("$$%u\n", msg_index);

	unsigned int total_msg_size = msg_size + _size;
	recv_msg = string(this->buff,this->buff+_size);
	unsigned int _recv_size = _size;

	while(_recv_size < total_msg_size)
	{
		_size = server.recv_msg(this->client_fd, min((unsigned int)BUFFSIZE,total_msg_size-_recv_size));
		if(_size <= 0)
			return _size;
		server.read_msg(this->buff, _size);
		recv_msg += string(this->buff,this->buff+_size);
		_recv_size += _size;
	}
	return msg_size;
}



int ui_comm::send_result(char result)
{
	char res = result;
	if(server.send_msg(this->client_fd, &res, 1) < 0)
		return -1;
	return 1;
}

int ui_comm::send_result(string result)
{
	int sz = result.size();
	char ssz[4];
	for(int i=0;i<4;i++){
		ssz[i] = sz % 256;
		sz /= 256;
		//printf("%d\n", ssz[i]);
	}
	//char + string 截断0
	string res = string(ssz,4) + result;
	if(server.send_msg(this->client_fd, res.c_str(), res.size()) < 0)
		return -1;
	return 1;
}
