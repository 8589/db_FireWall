#include "simple_comm.h"



int simple_comm::create_socket(int domain, int type, int protocol)
{
	if( (this->_socket = socket(domain, type, protocol)) == -1)
	{
		char msg[MSGSIZE];
		sprintf(msg, "create socket error:%s(errno:%d)\n", strerror(errno), errno);
		this->log.error(msg);
		//exit(0);
		return -1;
	}
	this->log.debug("create socket successfully");
	return 0;
}




void simple_comm::set_server(int domain, string addr, int port){
	memset(&(this->addr),0,sizeof(this->addr));
	this->addr.sin_family = domain;
	this->addr.sin_addr.s_addr = inet_addr(addr.c_str());
	this->addr.sin_port = htons(port);
}



int simple_comm::bind_socket(){
	int on = 1;
	if(setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
		return -1;
	if( bind(this->_socket, (sockaddr*)&(this->addr), sizeof(this->addr)) == -1 )
	{
		char msg[MSGSIZE];
		sprintf(msg, "bind socket error:%s(errno: %d)\n",strerror(errno),errno);
		this->log.error(msg);
		//exit(0);
		return -1;
	}
	this->log.debug("bind socket successfully");
	return 0;
}




int simple_comm::listen_client(int _size)
{
	if( listen(this->_socket, _size) == -1 )
	{
		char msg[MSGSIZE];
		sprintf(msg, "listen socket error:%s(errno: %d)\n",strerror(errno),errno);
		this->log.error(msg);
		//exit(0);
		return -1;
	}
	this->log.debug("listening .....");
	return 0;
}




int simple_comm::connect_to_server()
{
	if( connect(this->_socket, (sockaddr*)&(this->addr), sizeof(this->addr)) < 0 )
	{
		char msg[MSGSIZE];
		sprintf(msg, "connect socket error:%s(errno: %d)\n",strerror(errno),errno);
		this->log.error(msg);
		//exit(0);
		return -1;
	}
	this->log.debug("connect_to_server successfully");
	return 0;
}





int simple_comm::send_msg(const char* msg, int _size)
{
	return this->send_msg(this->_socket, msg, _size);
}

int simple_comm::send_msg(int _socket_, const char* msg, int _size)
{
	if( send(_socket_, msg, _size, 0) < 0 )
	{
		char msg[MSGSIZE];
		sprintf(msg, "send socket error:%s(errno: %d)\n",strerror(errno),errno);
		this->log.warning(msg);
		//exit(0);
		return -1;
	}
	this->log.debug("send msg successfully");
	return 0;
}





int simple_comm::recv_msg()
{
	return this->recv_msg(this->_socket,BUFFSIZE);
}

int simple_comm::recv_msg(int _socket_)
{
	return this->recv_msg(_socket_, BUFFSIZE);
}
int simple_comm::recv_msg(int _socket_, int _size)
{
	int recv_size = 0;
	if( (recv_size = recv(_socket_, this->buff, _size, 0)) < 0 )
	{
		char msg[MSGSIZE];
		sprintf(msg, "recv socket error:%s(errno: %d)\n",strerror(errno),errno);
		this->log.warning(msg);
		//exit(0);
		return -1;
	}
	this->log.debug("recv msg successfully");
	return recv_size;
}



void simple_comm::read_msg(char* buf)
{
	this->read_msg(buf,sizeof(this->buff));
}
void simple_comm::read_msg(char* buf, int _size)
{
	memcpy(buf, this->buff, _size);
}



int simple_comm::accept_client()
{
	int client;
	sockaddr_in client_addr;
	socklen_t clinet_addr_size = sizeof(client_addr);
	if( (client = accept(this->_socket, (sockaddr*)&client_addr, &clinet_addr_size)) == -1 )
	{
		char msg[MSGSIZE];
		sprintf(msg, "accept socket error:%s(errno: %d)\n",strerror(errno),errno);
		this->log.warning(msg);
		return -1;
		//exit(0);
	}
	char msg[MSGSIZE];
	sprintf(msg, "receive a connection from %s\n", inet_ntoa(client_addr.sin_addr));
	this->log.info(msg);
	return client;
}
int simple_comm::accept_client(sockaddr_in* client_addr)
{
	int client;
	socklen_t clinet_addr_size = sizeof(client_addr);
	if( (client = accept(this->_socket, (sockaddr*)client_addr, &clinet_addr_size)) == -1 )
	{
		char msg[MSGSIZE];
		sprintf(msg, "accept socket error:%s(errno: %d)\n",strerror(errno),errno);
		this->log.warning(msg);
		return -1;
		//exit(0);
	}
	char msg[MSGSIZE];
	sprintf(msg, "receive a connection from %s\n", inet_ntoa(client_addr->sin_addr));
	this->log.info(msg);
	return client;
}



void simple_comm::close_socket()
{
	close(this->_socket);
	char msg[MSGSIZE];
	sprintf(msg, "close socket\n");
	this->log.debug(msg);

}
void simple_comm::close_socket(int _socket_)
{
	close(_socket_);
	char msg[MSGSIZE];
	sprintf(msg, "close socket\n");
	this->log.debug(msg);
}
void simple_comm::close_socket(int _socket_, sockaddr_in* client_addr)
{
	close(_socket_);
	char msg[MSGSIZE];
	sprintf(msg, "close connect from %s\n", inet_ntoa(client_addr->sin_addr));
	this->log.info(msg);
}

int simple_comm::get_socket()
{
	return this->_socket;
}