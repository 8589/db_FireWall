#include <string>
#include <sys/epoll.h>
#include "config.h"
#include <unistd.h>

void myPrintError(const std::string &func);
void myExit(const std::string &func);
int getTCPServer_e(const std::string &ip, int port, int queueSize);
int getTCPServer_r(const std::string &ip, int port, int queueSize);
int getTCPClient_e(const std::string &ip, int port);
int getTCPClient_r(const std::string &ip, int port);

int recvAMsg(int fd, std::string &result);


int epollAddFd(int epfd, int fd, int events = EPOLLIN);

class FdGuard{
private:
	int fd;
public:
	FdGuard(int fd_) : fd(fd_){	}
	FdGuard(const FdGuard&) = delete;
	FdGuard& operator=(const FdGuard&) = delete;
	FdGuard(const FdGuard&&) = delete;
	FdGuard& operator=(const FdGuard&&) = delete;
	~FdGuard(){
		if(fd >= 0)
			close(fd);
	}
};