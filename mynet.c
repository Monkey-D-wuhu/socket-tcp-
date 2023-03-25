#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include "myfop.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

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

void *do_work(void *cfd)
{
	int connfd = (int)cfd;
	if(connfd < 0)
	{
		puts("connfd < 0");
		return NULL;
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
			//open
			int fd = open(filename, O_RDONLY);
			if(fd > 0)
			{
				//get filelen
				int fl = lseek(fd, 0, SEEK_END);
				char flen[20];
				memset(flen, 0, sizeof(flen));
				sprintf(flen,"%d",fl);
				//send filelen
				send(connfd, flen, sizeof(flen), 0);
				sleep(1);
				lseek(fd, 0, SEEK_SET);
				//read file and send
				char *pcon = (char *)malloc(fl);
				if(NULL == pcon)
				{
					puts("malloc error.");
					close(fd);
					return NULL;
				}
				ret = read(fd, pcon, fl);
				if(ret == fl)
				{
					send(connfd, pcon, fl, 0);
				}
				sleep(2);
				free(pcon);
				//close
				close(fd);
			}
			else
			{
				puts("open file error.");
			}
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
	close(connfd);
	return NULL;
}
