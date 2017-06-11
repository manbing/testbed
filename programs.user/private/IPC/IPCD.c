#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IPCD.h"

void do_start()
{
	printf("[%s][%d]\n", __func__, __LINE__);
}

void do_stop()
{
	printf("[%s][%d]\n", __func__, __LINE__);
}

void do_list()
{
	printf("[%s][%d]\n", __func__, __LINE__);
}

void do_delete()
{
	printf("[%s][%d]\n", __func__, __LINE__);
}

void do_exit()
{
	printf("[%s][%d]\n", __func__, __LINE__);
	exit(1);
}

int main(int argc , char *argv[])
{
	char cmd [2] = {0};
	struct action_t *action = NULL;
	int i = 0, input = 0;

	while(1)
	{
		action = action_menu;
		printf("---------IPCD actions menu-----------\n");
		for(i = 0; strlen(action->name) > 0;action++)
		{
			printf("[%d] %s - %s\n", i, action->name, action->description);
			i++;
		}
		printf("-------------------------------------\n");
		printf("Please enter index: ");
		fgets(cmd, 256, stdin);

		if((input = strtol(cmd, NULL, 10)) >= 0)
		{
			printf("Choose: %s\n", action_menu[input].name);
			if(action_menu[input].exe)
				action_menu[input].exe();
		}
		else
			printf("Input error!\n");
	}

	return 1;
}
