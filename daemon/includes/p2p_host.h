#ifndef P2P_HOST_H
#define P2P_HOST_H

#include <arpa/inet.h>

#include "p2p_opt.h"

#define NEVER 0

struct host
{
	unsigned short		opt;
	char							hash[20];
	struct in_addr		ip;
	unsigned short		port;
	time_t						last_connect;
	struct host				*next;
};

// p2p_host.c
struct host					*get_hosts(void);
void								init_hosts(char *ip, unsigned short port);
void								new_host(struct in_addr ip, unsigned short port);

#endif
