#ifndef CLIENT_H
#define CLIENT_H

struct infod
{
	int			socket;
};

// main.c
int							connect_daemon(void);
struct infod		*info_daemon(void);

// command.h
void						exec_cmd(char *cmd, char **arg);

#endif
