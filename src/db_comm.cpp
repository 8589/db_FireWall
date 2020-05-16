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


/*************************************function***********************************/
struct UserInfo{
	string user;
	string ip;
	UserInfo(const string &user_ = "", const string &ip_ = "") : user(user_), ip(ip_){}
};


static int login(int clientFd, int serverFd, char *buf, int bufSize, string &user){
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

static int IsMsgValid(const string& msg, filter* ftr, const UserInfo &userInfo){
	if(msg.size() < 5){
		return 0;
	}
	string sql = string(msg, 5, msg.size()-5);
printf("%s\n", sql.c_str());
	if(is_learning){
		ftr->add_white_list(userInfo.user, sql, userInfo.ip);
	}else{
		return ftr->is_legal(userInfo.user, sql, userInfo.ip);
	}
	return 1;
}


static void handleIllegalMsg(const string& msg, filter* ftr, const UserInfo &userInfo){
printf("user : %s\n ip : %s\n, SQL : %s", userInfo.user.c_str(), userInfo.ip.c_str(), string(msg, 5, msg.size()-5).c_str());
	ftr->log_illegal(userInfo.user, string(msg, 5, msg.size()-5), userInfo.ip);
}


static int epollCommunicate(int clientFd, int serverFd, char *buf, int bufSize, const UserInfo& userInfo){
	int epFd = epoll_create(2);
	if(epFd < 0){
		return -1;
	}
	FdGuard epFdGuard(epFd);

	if(epollAddFd(epFd, clientFd) < 0){
		return -1;
	}
	if(epollAddFd(epFd, serverFd) < 0){
		return -1;
	}

	epoll_event events[2];
	unique_ptr<filter> upFilter( new naive_filter ( shared_ptr<sql_parser>(new naive_sql_parser) ));

	while(1){
		int eventSize = epoll_wait(epFd, events, 2, -1);
		for(int i =0; i < eventSize; i ++){
			int thisFd = events[i].data.fd;
			if(thisFd == clientFd){
				string msg;
				int msgSize = recvAMsg(thisFd, msg);
				if(msgSize <= 0){
					return -1;
				}
				//check()
				if(!IsMsgValid(msg, upFilter.get(), userInfo)){
					//TODO
					handleIllegalMsg(msg, upFilter.get(), userInfo);
					return -1;
				}
				msgSize = send(serverFd, msg.c_str(), msgSize, 0);
				if(msgSize <= 0){
					return -1;
				}
			}else{
				if(transmitOnce(clientFd, serverFd, buf, bufSize) <= 0){
					return -1;
				}
			}
		}
	}
	return 1;
}

void handleDBConnection(int clientFd, sockaddr_in clientAddr){
	FdGuard clientFdGuard(clientFd);
	int serverFd = getTCPClient_r("127.0.0.1", server_port);
	if(serverFd < 0){
		return;
	}
	FdGuard serverFdGuard(serverFd);
	unique_ptr<char[]> upBuf(new char[buffsize]);
	UserInfo userInfo;
	char ipBuf[16];
	const char *result = inet_ntop(AF_INET, &clientAddr.sin_addr, ipBuf, INET_ADDRSTRLEN);
	if(!result){
		userInfo.ip = "unknown host";
	}
	userInfo.ip = result;
	if(login(clientFd, serverFd, upBuf.get(), buffsize, userInfo.user) < 0){
		return;
	}
printf("a connection from %s, user is %s\n", userInfo.ip.c_str(), userInfo.user.c_str());
	epollCommunicate(clientFd, serverFd, upBuf.get(), buffsize, userInfo);
}











/*************************************class***********************************/
int DBComm::IsMsgValid(){
	if(msg.size() < 5){
		return 0;
	}
	string sql = string(msg, 5, msg.size()-5);
printf("%s\n", sql.c_str());
	if(is_learning){
		ftr->add_white_list(user, sql, ip);
	}else{
		return ftr->is_legal(user, sql, ip);
	}
	return 1;
}


void DBComm::handleIllegalMsg(){
printf("user : %s\n ip : %s\n, SQL : %s", user.c_str(), ip.c_str(), string(msg, 5, msg.size()-5).c_str());
	ftr->log_illegal(user, string(msg, 5, msg.size()-5), ip);
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
		return;
	}

	if(epollAddFd(epFd, clientFd) < 0){
		return;
	}
	if(epollAddFd(epFd, serverFd) < 0){
		return;
	}

	epoll_event events[2];
	//TODO
	ftr = new naive_filter ( shared_ptr<sql_parser>(new naive_sql_parser) );

	while(1){
		int eventSize = epoll_wait(epFd, events, 2, -1);
		for(int i =0; i < eventSize; i ++){
			int thisFd = events[i].data.fd;
			if(thisFd == clientFd){
				int msgSize = recvAMsg(thisFd, msg);
				if(msgSize <= 0){
					return;
				}
				if(!IsMsgValid()){
					//TODO
					handleIllegalMsg();
					return;
				}
				msgSize = send(serverFd, msg.c_str(), msgSize, 0);
				if(msgSize <= 0){
					return;
				}
			}else{
				if(transmitOnce(clientFd, serverFd, buf, bufSize) <= 0){
					return;
				}
			}
		}
	}
}

void DBComm::handleDBConnection(){
	serverFd = getTCPClient_r("127.0.0.1", server_port);
	if(serverFd < 0){
		return;
	}
	//exception
	buf = new char[buffsize];
	bufSize = buffsize;
	if(login() < 0){
		return;
	}
printf("a connection from %s, user is %s\n", ip.c_str(), user.c_str());
	epollCommunicate();
}