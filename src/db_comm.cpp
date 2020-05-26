#include "db_comm.h"
#include <cstring>
#include<sys/epoll.h>
#include <iostream>
#include <chrono>
#include <memory>
#include <arpa/inet.h>
#include "filter.h"
#include "naive_filter.h"
#include "naive_sql_parser.h"
#include "utils.h"
#include <chrono>

using namespace std;


static int transmitOnce(int clientFd, int serverFd, char *buf, int bufSize){
	int recvSize = 0, sendSize = 0;
	//server -> client
	recvSize = recv(serverFd, buf, bufSize, 0);
	if(recvSize <= 0){
		return recvSize;
	}
	sendSize = send(clientFd, buf, recvSize, 0);
	if(send <= 0){
		return sendSize;
	}
	return recvSize;
}

/*************************************class***********************************/

inline void DBComm::init(){
	clientFd = -1;
	serverFd = -1;
	epFd = -1;
	ftr = nullptr;
	buf = nullptr;
	bufSize = 0;
	ip = "unknown host";
}

DBComm::DBComm(int clientFd_, sockaddr_in clientAddr){
	init();
	clientFd = clientFd_;
	char ipBuf[INET_ADDRSTRLEN];
	const char *result = inet_ntop(AF_INET, &clientAddr.sin_addr, ipBuf, INET_ADDRSTRLEN);
	if(result){
		ip = result;
	}
}


DBComm::~DBComm(){
	delete[] buf;
	delete ftr;
	if(clientFd >= 0)	close(clientFd);
	if(serverFd >= 0)	close(serverFd);
	if(epFd >= 0)	close(epFd);
	fwLogger->info("close a connection from {}, user is {}", ip, user);
}

int DBComm::IsMsgValid(){

	if(msg.size() < 5){
		return 0;
	}
	sql = string(msg, 5, msg.size()-5);
	fwLogger->debug("sql is {}", sql);
	if(is_learning){
		ftr->add_white_list(user, sql, ip);
	}else{
// auto start_time = std::chrono::steady_clock::now();
	ftr->is_legal(user, sql, ip);
// auto end_time = std::chrono::steady_clock::now();
// auto duration = std::chrono::duration_cast<chrono::microseconds>(end_time - start_time);
// std::cout << "The exec time is_legal() is : "<< duration.count() << "ms" << std::endl;
		return 1;
		//return ftr->is_legal(user, sql, ip);
	}
	return 1;
}


inline void DBComm::handleIllegalMsg(){
	if(is_log_illegal_query)
		ftr->log_illegal(user, sql, ip);
}


int DBComm::login(){
	if(transmitOnce(clientFd, serverFd, buf, bufSize) <= 0){
		return -1;
	}
	if(transmitOnce(serverFd, clientFd, buf, bufSize) <= 0){
		return -1;
	}
	user.clear();
	user = string(buf + 36);
	if(transmitOnce(clientFd, serverFd, buf, bufSize) <= 0){
		return -1;
	}
	return 1;
}


void DBComm::epollCommunicate(){
	epFd = epoll_create(2);
	if(epFd < 0){
		myPinrtWarn("epoll_create");
		return;
	}

	if(epollAddFd(epFd, clientFd) < 0){
		myPinrtWarn("epollAddFd");
		return;
	}
	if(epollAddFd(epFd, serverFd) < 0){
		myPinrtWarn("epollAddFd");
		return;
	}

	epoll_event events[2];
	ftr = new naive_filter ( shared_ptr<sql_parser>(new naive_sql_parser) );
	while(1){
		int eventSize = epoll_wait(epFd, events, 2, -1);
		for(int i =0; i < eventSize; i ++){
			int thisFd = events[i].data.fd;
			if(thisFd == clientFd){
// auto start_time = std::chrono::steady_clock::now();
				int msgSize = recvAMsg(thisFd, msg);
				if(msgSize <= 0){
					return;
				}
// auto end_time = std::chrono::steady_clock::now();
// auto duration = std::chrono::duration_cast<chrono::microseconds>(end_time - start_time);
// std::cout << "The exec time recvAMsg() is : "<< duration.count() << "ms" << std::endl;
// start_time = std::chrono::steady_clock::now();
				if(!IsMsgValid()){
					handleIllegalMsg();
					return;
				}
// end_time = std::chrono::steady_clock::now();
// duration = std::chrono::duration_cast<chrono::microseconds>(end_time - start_time);
// std::cout << "The exec time IsMsgValid() is : "<< duration.count() << "ms" << std::endl;
// start_time = std::chrono::steady_clock::now();
				msgSize = send(serverFd, msg.c_str(), msgSize, 0);
				if(msgSize <= 0){
					return;
				}
// end_time = std::chrono::steady_clock::now();
// duration = std::chrono::duration_cast<chrono::microseconds>(end_time - start_time);
// std::cout << "The exec time send() is : "<< duration.count() << "ms" << std::endl;
			}else{
				if(transmitOnce(clientFd, serverFd, buf, bufSize) <= 0){
					return;
				}
			}
		}
	}
}

void DBComm::handleDBConnection(){
	serverFd = getTCPClient_w("127.0.0.1", server_port);
	if(serverFd < 0){
		myPinrtWarn("getTCPClient_w");
		return;
	}
	//exception
	buf = new char[buffsize];
	bufSize = buffsize;
	if(login() < 0){
		myPinrtWarn("login");
		return;
	}
fwLogger->info("a connection from {}, user is {}", ip, user);
	epollCommunicate();
}

