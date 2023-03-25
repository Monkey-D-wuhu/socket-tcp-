#include "mycli.h"
#include <stdio.h>
#include <unistd.h>

int main()
{
	int sock_fd = init_cli("127.0.0.1", 9999);
	if(sock_fd < 0)
	{
		puts("init cli error.");
		return -1;
	}
	puts("init cli success.");
	do_work(sock_fd);
	close(sock_fd);
	return 0;
}
