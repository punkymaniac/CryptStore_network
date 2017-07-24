#ifndef COMMAND_H
#define COMMAND_H

struct			cmd
{
	char			*name;
	void			(*func)(char **arg);
};

// command.c
void				c_exit(char **arg);

#endif
