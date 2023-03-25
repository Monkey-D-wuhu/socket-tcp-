#include "mynet.h"
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

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
	while(1)
	{
		int connfd = accept_cli(ser_fd);
		if(connfd < 0)
		{
			puts("accept_cli fail");
			close(ser_fd);
			return -1;
		}
		puts("accept cli success.");
		
		//创建线程
		pthread_t tid;
		int ret = pthread_create(&tid, NULL, do_work, (void *)connfd);
		if(ret != 0)
		{
			puts("pthread create error.");
			continue;
		}
		pthread_detach(tid);
	}
	close(ser_fd);
	return 0;
}
