#ifndef P2P_H
#define P2P_H

// p2p.c
int					create_p2p_socket(int port);
void				recv_new_connection(int socket);

#endif
