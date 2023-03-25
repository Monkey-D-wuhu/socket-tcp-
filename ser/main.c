#include "mynet.h"
#include <unistd.h>
#include <stdio.h>

int main()
{
	int ser_fd = init_net(9999);
	if(ser_fd < 0)
	{
		puts("init_net fail.");
		close(ser_fd);
		return -1;
	}
	puts("init_net success.");
	int connfd = accept_cli(ser_fd);
	if(connfd < 0)
	{
		puts("accept_cli fail");
		close(ser_fd);
		return -1;
	}
	puts("accept cli success.");
	do_work(connfd);
	close(connfd);
	close(ser_fd);
	return 0;
}
