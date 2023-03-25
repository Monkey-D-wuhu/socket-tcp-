#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include "myfop.h"

int init_net(unsigned short port)
{
	//socket
	int fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0)
	{
		puts("socket error.");
		return -1;
	}
	puts("socket success.");
	//bind
	struct sockaddr_in myser;
	memset(&myser, 0, sizeof(myser));
	myser.sin_family = AF_INET;
	myser.sin_port = htons(port);
	myser.sin_addr.s_addr = htonl(INADDR_ANY);
	int ret = bind(fd, (struct sockaddr *)&myser, sizeof(myser));
	if(ret != 0)
	{
		puts("bind error.");
		close(fd);
		return -1;
	} 
	puts("bind success.");
	//listen
	ret = listen(fd, 5);
	if(ret != 0)
	{
		puts("listen error.");
		close(fd);
		return -1;
	} 
	puts("listen success.");
	return fd;
}

int accept_cli(int serfd)
{
	if(serfd < 0)
	{
		puts("accept cli error.");
		return -1;
	}
	int connfd = -1;
	struct sockaddr_in mycli;
	int len = sizeof(mycli);
	connfd = accept(serfd, (struct sockaddr *)&mycli, &len);
	if(connfd < 0)
	{
		puts("accept cli error.");
		return -1;
	}
	return connfd;
}

int do_work(int connfd)
{
	if(connfd < 0)
	{
		puts("connfd < 0");
		return -1;
	}
	puts("do-work");
	char cmd[50];
	memset(cmd, 0, sizeof(cmd));
	int ret = recv(connfd, cmd, sizeof(cmd), 0);
	if(ret > 0)
	{
		puts(cmd);
		char req[5] = {0};
		char filename[50] = {0};
		anlayse_cmd(cmd, req, filename);
		if(strcmp(req,"get") == 0)
		{
			puts("download file");
			puts(filename);
		}
	}	
	else if(ret == 0)
	{
		puts("client close");
	}
	else
	{
		puts("recv error.");
	}
	return 0;
}
