#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#include "p2p.h"
#include "p2p_host.h"

static struct host		**st_get_host(void)
{
	static struct host	*hostlst = 0x00;

	return (&hostlst);
}

struct host					*get_hosts(void)
{
	return (*(st_get_host()));
}

void								init_hosts(char *ip, unsigned short port)
{
	struct host				**hostlst;
	struct host				*host;

	hostlst = st_get_host();
	host = (struct host *)malloc(sizeof(struct host));
	host->opt = OPT_CURRENT;
	memcpy(host->hash, "testHash\0", 9);
	inet_aton(ip, &(host->ip));
	host->port = port;
	host->last_connect = NEVER;
	host->next = 0x00;
	*hostlst = host;
}

void								new_host(struct in_addr ip, unsigned short port)
{
	int								i;
	struct host				*hostlst;
	struct host				*newhost;
	int								sock;

	hostlst = get_hosts();
	for (i = 0; hostlst->next != 0x00; ++i)
		hostlst = hostlst->next;
	newhost = (struct host *)malloc(sizeof(struct host));
	newhost->opt = OPT_DEFAULT;
	memcpy(newhost->hash, "testHash\0", 9);
	newhost->ip = ip;
	newhost->port = port;
	newhost->last_connect = NEVER;
	newhost->next = 0x00;
	hostlst->next = newhost;
	// test connexion with host
	sock = connect_p2p(newhost->ip, newhost->port);
	if (sock != -1)
	{
		#ifndef DEBUG
		dprintf(1, "CONNECTED to p2p host\n");
		#endif
		newhost->last_connect = time(0x00);
	}
	#ifndef DEBUG
	else
		dprintf(1, "ERROR join p2p host\n");
	#endif

	close(sock);
}
