#ifndef _DB_COMM_H_
#define _DB_COMM_H_

#include "simple_comm.h"
#include "filter.h"
#include <vector>
#include <string>
#include <memory>
#include <arpa/inet.h>

using namespace std;
extern atomic<bool> is_learning;
extern int is_log_illegal_query;


void handleDBConnection(int clientFd, sockaddr_in clientAddr);


class DBComm{
private:

	string user;
	string ip;
	string msg;
	string sql;


	int clientFd;
	int serverFd;

	int epFd;

	filter *ftr;

	char *buf;
	int bufSize;

	

	void init(){
		clientFd = -1;
		serverFd = -1;
		epFd = -1;
		ftr = nullptr;
		buf = nullptr;
		bufSize = 0;
		ip = "unknown host";
	}
	int login();
	void epollCommunicate();
	int IsMsgValid();
	void handleIllegalMsg();

public:
	DBComm(int clientFd_, sockaddr_in clientAddr){
		init();
		clientFd = clientFd_;
		char ipBuf[INET_ADDRSTRLEN];
		const char *result = inet_ntop(AF_INET, &clientAddr.sin_addr, ipBuf, INET_ADDRSTRLEN);
		if(result){
			ip = result;
		}
	}
	
	~DBComm(){
		delete[] buf;
		delete ftr;
		if(clientFd >= 0)	close(clientFd);
		if(serverFd >= 0)	close(serverFd);
		if(epFd >= 0)	close(epFd);
//printf("close a connection from %s, user is %s\n", ip.c_str(), user.c_str());
	}

	void handleDBConnection();

};


#endif
