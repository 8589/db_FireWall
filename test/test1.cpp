#include "simple_comm.h"
#include <cstdio>


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
        while((len=client.recv_msg())>0){
        	client.read_msg(buff);
        	if(server.send_msg(connfd, buff, len) < 0){
        		printf("send1 error\n");
        		exit(6);
        	}
        	printf("send client\n");
        	len=server.recv_msg(connfd);
            server.read_msg(buff);
        	printf("from client: %d Bytes\n", len);
        	if(client.send_msg(buff, len)<0){
        		printf("send2 error\n");
        		exit(7);
        	}
        }
        
    }

}

