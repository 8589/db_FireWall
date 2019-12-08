#include "simple_comm.h"
#include <cstdio>
int main()
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(6666);

    if(bind(listenfd, (sockaddr*)&servaddr, sizeof(servaddr))<0){
    	printf("bind error\n");
    	exit(1);
    }


    if(listen(listenfd, 10)<0){
    	printf("listen error\n");
    	exit(2);
    }

    while(1)
    {
    	printf("listening....\n");
        sockaddr_in cliaddr;
        socklen_t clilen = sizeof(cliaddr);
        int connfd = accept(listenfd, (sockaddr*)&cliaddr, &clilen);
        if(connfd < 0){
        	printf("accept error\n");
        	exit(3);
        }
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd < 0){
        	printf("socket2 error\n");
        	exit(4);
        }
        sockaddr_in clientaddr;
        memset(&clientaddr, 0, sizeof(clientaddr));
        clientaddr.sin_family = AF_INET;
        clientaddr.sin_port = htons(3306);
        inet_pton(AF_INET, "127.0.0.1", &clientaddr.sin_addr);
        if(connect(sockfd, (sockaddr*)&clientaddr, sizeof(clientaddr)) < 0){
        	printf("connect error\n");
        	exit(5);
        }
        char buff[4096];
        int len = 0;
        while((len=recv(sockfd, buff, sizeof(buff), 0)) > 0){
        	printf("from server: %d Bytes\n", len);
        	if(send(connfd, buff, len, 0) < 0){
        		printf("send1 error\n");
        		exit(6);
        	}
        	printf("send client\n");
        	len=recv(connfd, buff,sizeof(buff),0);
        	printf("from client: %d Bytes\n", len);
        	if(send(sockfd,buff,len,0)<0){
        		printf("send2 error\n");
        		exit(7);
        	}
        }
        
    }

}

