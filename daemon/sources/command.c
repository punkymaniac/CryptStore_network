#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "glob/core.h"
#include "p2p_host.h"
#include "localclient.h"
#include "command.h"

static struct cmd			*st_get_cmd(void)
{
	/*
	array doit avoir le meme ordre que array dans st_get_cmd du fichier
	client/sources/localclient.c
	*/
	static struct cmd		array[10] = {
		{"addhost", &c_addhost},
		{"listhost", &c_listhost},
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

void					c_addhost(char *data)
{
	struct in_addr		ip;
	unsigned short		port;

	memcpy(&ip, data, sizeof(struct in_addr));
	data += sizeof(struct in_addr);
	memcpy(&port, data, sizeof(unsigned short));
	dprintf(1, "ip: %s:%d\n", inet_ntoa(ip), port);
	new_host(ip, port);
}

void					c_listhost(char *data)
{
	(void)data;
	struct host		*hostlst;
	short					nbrhost;
	int						sizehosts;
	char					*buffer;
	int						sizebuffer;

	nbrhost = 0;
	sizebuffer = 0;
	hostlst = get_hosts();
	while (hostlst)
	{
		if ((hostlst->opt & OPT_CURRENT) == 0)
			++nbrhost;
		hostlst = hostlst->next;
	}
	sizehosts = nbrhost * sizeof(struct host);
	buffer = (char *)malloc((sizeof(char) * sizehosts) + 1);
	buffer[sizehosts] = '\0';
	hostlst = get_hosts();
	while (hostlst)
	{
		if ((hostlst->opt & OPT_CURRENT) == 0)
		{
			memcpy(&buffer[sizebuffer], hostlst, sizeof(struct host));
			sizebuffer += sizeof(struct host);
		}
		hostlst = hostlst->next;
	}
	send_client(buffer, sizebuffer + 1);
	free(buffer);
}
