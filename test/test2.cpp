#include "simple_comm.h"
#include <cstdio>

int cnt = 0;
int recv_a_packet(string& recv_msg, int _socket_)
{
    simple_comm server;
    recv_msg.clear();
    //get the size of this packet
    int _size = server.recv_msg(_socket_, 4);
    if(_size  < 0)
        return -1;

    if(!_size)  return 0;

    char buff[4096];
    
    server.read_msg(buff, _size);

    //get the size of real msg 
    unsigned int msg_size = (*((unsigned int*)buff))&(0x00ffffffu);

    //get the size of real packet
    unsigned int total_msg_size = msg_size + 4;
    recv_msg = string(buff,buff+_size);
    unsigned int _recv_size = _size;

    while(_recv_size < total_msg_size)
    {
        if((_size = server.recv_msg(_socket_, min((unsigned int)BUFFSIZE, total_msg_size-_recv_size))) < 0)
            return -1;
        server.read_msg(buff, _size);
        recv_msg += string(buff,buff+_size);
        _recv_size += _size;
    }
    return msg_size;
}



int select_one_comm(int server, int client)
{
    simple_comm sc;
    fd_set rset;
    FD_ZERO(&rset);
    FD_SET(server, &rset);
    FD_SET(client, &rset);
    int maxfd = std::max(client, server) + 1;
    timeval tv;
    tv.tv_sec = 60;
    int res = select(maxfd, &rset, nullptr, nullptr, &tv);
    string s;
    if(FD_ISSET(client, &rset)){
        int _size = recv_a_packet(s, client);
        printf("%d times from client : %d Bytes\n",cnt++, _size);
        if(_size<=0)
            return 0;
        sc.send_msg(server, s.c_str(), s.size());
    }
    if(FD_ISSET(server, &rset)){
        int _size = sc.recv_msg(server);
        printf("%d times from client : %d Bytes\n",cnt, _size);
        char buff[4096];
        sc.read_msg(buff,_size);
        if(_size<=0)
            return 0;
        sc.send_msg(client, buff, _size);
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

