#ifndef COMMAND_H
#define COMMAND_H

struct			cmd
{
	char			*name;
	void			(*func)(short idcmd, char **arg);
};

// command.c
void				c_exit(short idcmd, char **arg);
void				c_addhost(short idcmd, char **arg);

#endif
