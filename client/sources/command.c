#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "glob/core.h"
#include "command.h"

static struct cmd			*st_get_cmd(void)
{
	static struct cmd		array[10] = {
		{"exit", &c_exit},
		{0x00, 0x00}
	};
	return (array);
}

static short					st_search_cmd(char *str)
{
	struct cmd			*acmd;
	short						pos;

	acmd = st_get_cmd();
	for (pos = 0; acmd[pos].name != 0x00; ++pos)
	{
		if (strcmp(str, acmd[pos].name) == 0)
			return (pos);
	}
	return (-1);
}

void					run_cmd(char *cmd, char **arg)
{
	struct cmd	*acmd;
	short				idcmd;

	acmd = st_get_cmd();
	idcmd = st_search_cmd(cmd);
	if (idcmd == -1)
		dprintf(2, "%s: command not found\n", cmd);
	else
	{
		#ifdef DEBUG
			dprintf(1, "Execute command: %s\n", cmd);
		#endif
		acmd[idcmd].func(arg);
	}
}

void					c_exit(char **arg)
{
	(void)arg;
	#ifdef DEBUG
		dprintf(1, "=> Exit client\n");
	#endif
	exit(0);
}
