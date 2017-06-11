#ifndef __IPCD_H__
#define __IPCD_H__

#include <stddef.h>

void do_start();
void do_stop();
void do_list();
void do_delete();
void do_exit();

struct action_t
{
	char name [18];
	char description [256];
	void (*exe)(void);
};

struct action_t action_menu [] =
{
	{
		.name="start",
		.description = "Lanuch IPC center in order to accpet connection",
		.exe=do_start,
	},
	{
		.name="stop",
		.description = "Stop IPC center and relase all connection",
		.exe=do_stop,
	},
	{
		.name="list",
		.description = "Display all connection",
		.exe=do_list,
	},
	{
		.name="delete",
		.description = "Close specific connection",
		.exe=do_delete,
	},
	{
		.name="exit",
		.description = "Exit this process",
		.exe=do_exit,
	},
	{
		.name="",
		.exe=NULL,
	},
};

#endif
