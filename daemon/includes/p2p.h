#ifndef P2P_H
#define P2P_H

#include <arpa/inet.h>

// p2p.c
int					create_p2p_socket(int port);
int					connect_p2p(struct in_addr ip, int port);
void				recv_new_connection(int socket);

#endif
