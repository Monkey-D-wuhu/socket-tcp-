#ifndef __MYNET_H__
#define __MYNET_H__
	int init_net(unsigned short port);
	int accept_cli(int serfd);
	int do_work(int connfd);
#endif
