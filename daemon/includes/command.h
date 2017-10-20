#ifndef COMMAND_H
#define COMMAND_H

struct			cmd
{
	char			*name;
	void			(*func)(char *data);
};

// command.c
void				process_cmd(char *data, int size);
void				c_addhost(char *data);
void				c_listhost(char *data);

#endif
