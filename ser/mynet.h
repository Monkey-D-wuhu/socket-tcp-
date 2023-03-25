#ifndef __MYNET_H__
#define __MYNET_H__
	int init_net(unsigned short port);
	int accept_cli(int serfd);
	void *do_work(void *cfd);
#endif
