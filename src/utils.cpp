#include <cstdio>
#include "utils.h"
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

using namespace std;

inline void myPrintError(const string &func){
	fwLogger->error("{}() error:{}(errno:{})", func, strerror(errno), errno);
}

inline void myPinrtWarn(const string &func){
	fwLogger->warn("{}() error:{}(errno:{})", func, strerror(errno), errno);
}

inline void myExit(const string &func){
	myPrintError(func);
	exit(-1);
}


static void setAddr_e(sockaddr_in *addr, int domain, const string &ip, int port){
	memset(addr, 0, sizeof(sockaddr_in));
	addr->sin_family = domain;
	if(ip.empty()){
		addr->sin_addr.s_addr = htonl(INADDR_ANY);
	}else{
		if(inet_pton(AF_INET, ip.c_str(), &(addr->sin_addr)) != 1){
			myExit("inet_pton");
		}
	}
	addr->sin_port = htons(port);
}


static int setAddr_w(sockaddr_in *addr, int domain, const string &ip, int port){
	memset(addr, 0, sizeof(sockaddr_in));
	addr->sin_family = domain;
	if(ip.empty()){
		addr->sin_addr.s_addr = htonl(INADDR_ANY);
	}else{
		if(inet_pton(AF_INET, ip.c_str(), &(addr->sin_addr)) != 1){
			myPinrtWarn("inet_pton");
			return -1;
		}
	}
	addr->sin_port = htons(port);
	return 0;
}

int getTCPServer_e(const string &ip, int port, int queueSize){
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0){
		myExit("socket");
	}
	sockaddr_in addr;
	setAddr_e(&addr, AF_INET, ip, port);
	int on = 1;
	if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))){
		myExit("setsockopt");
	}
	if(bind(fd, (sockaddr*)&(addr), sizeof(sockaddr_in))){
		myExit("setsockopt");
	}
	if(listen(fd, queueSize)){
		myExit("listen");
	}
	return fd;
}

int getTCPServer_w(const string &ip, int port, int queueSize){
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0){
		myPinrtWarn("socket");
		return -1;
	}
	sockaddr_in addr;
	if(setAddr_w(&addr, AF_INET, ip, port)){
		return -1;
	}
	int on = 1;
	if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))){
		myPinrtWarn("setsockopt");
		return -1;
	}
	if(bind(fd, (sockaddr*)&(addr), sizeof(sockaddr_in))){
		myPinrtWarn("setsockopt");
		return -1;
	}
	if(listen(fd, queueSize)){
		myPinrtWarn("listen");
		return -1;
	}
	return fd;
}


int getTCPClient_e(const string &ip, int port){
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0){
		myExit("socket");
	}
	sockaddr_in addr;
	setAddr_e(&addr, AF_INET, ip, port);
	if(connect(fd, (sockaddr*)&(addr), sizeof(sockaddr_in))){
		myExit("socket");
	}
	return fd;
}


int getTCPClient_w(const string &ip, int port){
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0){
		myPinrtWarn("socket");
		return -1;
	}
	sockaddr_in addr;
	if(setAddr_w(&addr, AF_INET, ip, port)){
		return -1;
	}
	if(connect(fd, (sockaddr*)&(addr), sizeof(sockaddr_in))){
		myPinrtWarn("connect");
		return -1;
	}
	return fd;
}


int recvAMsg(int fd, string &result)
{
	result.clear();
	
	char buf[BUFFSIZE];

	int recvSize = recv(fd, buf, BUFFSIZE, 0);

	if(recvSize <= 0){
		return recvSize;
	}

	//unsigned long msgSize = ntohl();
	unsigned long msgSize = ((*((unsigned int*)buf))&(0x00ffffffu)) + 4;

	unsigned long totalRecvSize = recvSize;

	result = string(buf, buf+recvSize);

	while(totalRecvSize < msgSize){
		if((recvSize = recv(fd, buf, BUFFSIZE, 0)) < 0){
			return -1;
		}
		totalRecvSize += recvSize;
		result += string(buf, buf+recvSize);
	}
	return msgSize;
}







int epollAddFd(int epfd, int fd, int events){
	epoll_event ev;
	ev.data.fd = fd;
	ev.events = events;
	return epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}


