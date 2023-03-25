#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int init_cli(char *ip, unsigned short port)
{
	if(NULL == ip)
	{
		puts("NULL pointer error.");
		return -1;
	}
	int connfd = -1;
	connfd = socket(AF_INET, SOCK_STREAM, 0);
	if(connfd < 0)
	{
		puts("socket error.");
		return -1;
	}
	puts("socket success.");
	struct sockaddr_in myser;
	memset(&myser, 0, sizeof(myser));
	myser.sin_family = AF_INET;
	myser.sin_port = htons(port);
	myser.sin_addr.s_addr = inet_addr(ip);
	int ret = connect(connfd, (struct sockaddr *)&myser, sizeof(myser));
	if(ret < 0)
	{
		puts("connect server error.");
		close(connfd);
		return -1;
	}
	puts("connect server success.");
	return connfd;
}

int do_work(int connfd)
{
	if(connfd < 0)
	{
		puts("connfd < 0");	
		return -1;
	}
	puts("do_work");
	char cmd[50];
	memset(cmd, 0, sizeof(cmd));
	fgets(cmd,sizeof(cmd),stdin);
	cmd[strlen(cmd)-1] = 0;
	send(connfd,cmd,sizeof(cmd),0);
	char filename[50] = {0};
	strcpy(filename, cmd+4);
	puts(filename);
	sleep(3);
	int fd = open(filename, O_CREAT | O_WRONLY, 0664);
	if(fd < 0)
	{
		puts("open file error.");
		close(connfd);
		return -1;
	}
	char flen[20] = {0};
	int ret = recv(connfd, flen, sizeof(flen), 0);
	if(ret > 0)
	{
		int fl = atoi(flen);
		char *p = (char *)malloc(fl);
		while(fl)
		{
			memset(p,0,fl);
			ret = recv(connfd, p, fl, 0);
			if(ret > 0)
			{
				write(fd,p,ret);
				printf("ret:%d\n",ret);
				fl -= ret;
			}
			else if(ret == 0)
			{
				break;
			}
		}
	}
	close(fd);
	close(connfd);
	return 0;
}
