#ifndef P2P_HOST_H
#define P2P_HOST_H

#include <arpa/inet.h>

#include "glob/host_struct.h"

// p2p_host.c
struct host					*get_hosts(void);
void								init_hosts(char *ip, unsigned short port);
void								new_host(struct in_addr ip, unsigned short port);

#endif
