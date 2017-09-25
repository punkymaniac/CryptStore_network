#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "glob/core.h"
#include "command.h"

static struct cmd			*st_get_cmd(void)
{
	/*
	array doit avoir le meme ordre que array dans st_get_cmd du fichier
	client/sources/localclient.c
	*/
	static struct cmd		array[10] = {
		{"exit", 0x00},
		{0x00, 0x00}
	};
	return (array);
}

void					process_cmd(char *data, int size)
{
	struct cmd	*acmd;
	short				idcmd;

	(void)size;
	acmd = st_get_cmd();
	memcpy(&idcmd, data, 2);
	dprintf(1, "idcmd: %d\n", idcmd);
	acmd[idcmd].func(data + sizeof(idcmd));
}
