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

	string user, ip, msg, sql;

	int clientFd, serverFd, epFd;

	filter *ftr;

	char *buf;
	int bufSize;

	void init();

	int login();
	void epollCommunicate();
	int IsMsgValid();
	void handleIllegalMsg();

public:
	DBComm(int clientFd_, sockaddr_in clientAddr);
	
	~DBComm();

	void handleDBConnection();

};


#endif
