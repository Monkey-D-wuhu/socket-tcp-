#include <stdio.h>
#include <string.h>

int anlayse_cmd(char *cmd, char *req, char *filename)
{
	if(NULL == cmd || NULL == req || NULL == filename)
	{
		puts("NULL pointer error.");
		return -1;
	}
	char *p = cmd;
	int i = 0;
	for(i = 0; i < 3; i++)
	{
		req[i] = cmd[i]; 
	}
	req[i] = 0;
	strcpy(filename, cmd+4);
	return 0;
}
