#include "firewall.h"
#include <map>
#include <string>
#include <list>
#include <sys/epoll.h>


const int MAX_EVENT_NUMBER  = 2048;

using namespace std;

struct Descriptor{
	int fd;
	//1 client 2 server
	char type;
	//0 nothing 1 connect server 2 server->client 3 client -> server 4 server->client 5 establish
	char state = 0;
	sockaddr_in addr;
	Descriptor(int fd_, int type_, sockaddr_in addr_) : fd(fd_), type(type_), addr(addr_){}
	Descriptor(int type_) : type(type_){}
	Descriptor(){}
};
struct msg{
	int fd;
	char type;
	string data;
	msg(int fd_, char type_, const string &data_) : fd(fd_), type(type_), data(data_){}
};

map<int, Descriptor> portMap;
list<msg> msgQueue;


void start(int db_server_port, int firewall_port, int ui_comm_port){
	int fw = getTCPServer_e("", 6666, listen_queue_size);
	int ui = getTCPServer_e("", 6667, listen_queue_size);

	int epfd = epoll_create(2048);
	if(epfd < 0){
		myExit("epoll_create");
	}

	epollAddFd(epfd, fw);

	epoll_event events[MAX_EVENT_NUMBER];

	while(1){
		int nfds = epoll_wait(epfd, events, MAX_EVENT_NUMBER, -1);
		if(nfds < 0){
			myExit("epoll_wait");
		}
		for(int i = 0; i < nfds; ++ i){
			int thisFd = events[i].data.fd;
			if(thisFd == fw){
				Descriptor descor(1);
				descor.state = 1;
				socklen_t addrSize = sizeof(descor.addr);
				int client = accept(fw, (sockaddr*)&(descor.addr), &addrSize);

				if(!client){
printf("accept error\n");
					continue;
				}
printf("receive a connection\n");
				descor.fd = getTCPClient_r("127.0.0.1", 3306);

				if(!(descor.fd)){
printf("connect server error\n");
					close(client);
					continue;
				}
printf("connect mysql server\n");

				epollAddFd(epfd, client);
				epollAddFd(epfd, descor.fd);
printf("%d %d\n", client, descor.fd);

				portMap[client] = descor;

				swap(descor.fd, client);
				descor.type = 2;
printf("%d %d\n", client, descor.fd);
				portMap[client] = descor;

				//msgQueue.emplace_back(descor.fd, 1, "");
			}else{
				if(events[i].events & EPOLLIN){
					auto iter = portMap.find(thisFd);
					if(iter == portMap.end()){
						continue;
					}
					if(iter->second.type == 1){

						string msg;
						int msgSize = recvAMsg(thisFd, msg);

printf("receive a msg from client %d\n", msgSize);
						if(msgSize < 0){
							continue;
						}else if(msgSize == 0){
							close(thisFd);
							close(iter->second.fd);
							portMap.erase(thisFd);
							portMap.erase(iter->second.fd);
							continue;
						}
						if(iter->second.state == 5){
							//check()
							msgSize = send(iter->second.fd, msg.c_str(), msgSize, 0);
printf("send a msg to client %d\n", msgSize);		
						}else{
							msgSize = send(iter->second.fd, msg.c_str(), msgSize, 0);
printf("send a msg to client %d\n", msgSize);	
							iter->second.state++;
							portMap[iter->second.fd].state++;
						}

					}else if(iter->second.type == 2){

						char buf[BUFFSIZE];
						int msgSize = recv(thisFd, buf, BUFFSIZE, 0);
printf("receive a msg from server %d\n", msgSize);
						msgSize = send(iter->second.fd, buf, msgSize, 0);
printf("send a msg to client %d\n", msgSize);		
						if(iter->second.state != 5){
							iter->second.state++;
							portMap[iter->second.fd].state++;
						}
					}
				}
			}
		}
	}
}



// /insert into t1 select a+20, b+20, c+20 from t1;












void firewall::start_firewall(int db_server_port, int firewall_port)
{
	simple_comm server;
	if(server.create_socket(AF_INET, SOCK_STREAM, 0) < 0){
		exit(-1);
	}
	server.set_server(AF_INET,"0.0.0.0",firewall_port);
	if(server.bind_socket() < 0){
		exit(-1);
	}
	if(server.listen_client(listen_queue_size) < 0){
		exit(-1);
	}
	while(1){
		sockaddr_in client_addr;
		int client = server.accept_client(&client_addr);
		if(client < 0){
			continue;
		}
		thread t([](int _client, int port, sockaddr_in _client_addr, bool _mode){
			db_comm dc(_client, port, _client_addr, _mode, shared_ptr<filter>( new naive_filter ( shared_ptr<sql_parser>(new naive_sql_parser) )));
			dc.handle_db_connection();
		}, client, db_server_port, client_addr, is_learning.load());
		t.detach();
	}
	server.close_socket();
}

void firewall::comm_with_web_UI(int db_server_port, int ui_comm_port)
{
	simple_comm server;
	if(server.create_socket(AF_INET, SOCK_STREAM, 0) < 0){
		exit(-1);
	}
	server.set_server(AF_INET,"0.0.0.0", ui_comm_port);
	if(server.bind_socket() < 0){
		exit(-1);
	}
	if(server.listen_client(listen_queue_size) < 0){
		exit(-1);
	}
	while(1){
		sockaddr_in client_addr;
		int client = server.accept_client(&client_addr);
		if(client < 0){
			continue;
		}
		thread t([](int _client, int port, sockaddr_in _client_addr){
			ui_comm uc(_client, port);
			uc.handle_ui_connection();
		}, client, db_server_port, client_addr);
		t.detach();
	}
	server.close_socket();
}