#include "simple_comm.h"
#include <cstdio>


int select_one_comm(int server, int client)
{   simple_comm sc;
    fd_set rset;
    FD_ZERO(&rset);
    FD_SET(server, &rset);
    FD_SET(client, &rset);
    int maxfd = std::max(client, server) + 1;
    timeval tv;
    tv.tv_sec = 60;
    int res = select(maxfd, &rset, nullptr, nullptr, &tv);
    string s;
    char buff[4096];
    if(FD_ISSET(client, &rset)){
        int len = sc.recv_msg(client);
        sc.read_msg(buff,len);
        if(len<=0)
            return 0;
        sc.send_msg(server, buff, len);
    }
    if(FD_ISSET(server, &rset)){
        int len = sc.recv_msg(server);
        sc.read_msg(buff,len);
        if(len<=0)
            return 0;
        sc.send_msg(client, buff, len);
    }
    return 1;
}

int main()
{
    simple_comm server;
    server.create_socket(AF_INET, SOCK_STREAM, 0);


    server.set_server(AF_INET, "0.0.0.0", 6666);


    if(server.bind_socket()<0){
    	printf("bind error\n");
    	exit(1);
    }


    if(server.listen_client(10)<0){
    	printf("listen error\n");
    	exit(2);
    }

    while(1)
    {
    	printf("listening....\n");
        int connfd = server.accept_client();

        simple_comm client;
        client.create_socket(AF_INET, SOCK_STREAM, 0);
        client.set_server(AF_INET, "127.0.0.1", 3306);
        client.connect_to_server();

        char buff[4096];
        int len = 0;
        while(select_one_comm(client.get_socket(), connfd)>0);
    }

}

