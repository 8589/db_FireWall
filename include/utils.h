#include <string>
#include <sys/epoll.h>
#include "config.h"

void myPrintError(const std::string &func);
void myExit(const std::string &func);
int getTCPServer_e(const std::string &ip, int port, int queueSize);
int getTCPServer_r(const std::string &ip, int port, int queueSize);
int getTCPClient_e(const std::string &ip, int port);
int getTCPClient_r(const std::string &ip, int port);

int recvAMsg(int fd, std::string &result);


void epollAddFd(int epfd, int fd, int events = EPOLLIN);