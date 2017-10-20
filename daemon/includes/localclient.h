#ifndef LOCALCLIENT_H
#define LOCALCLIENT_H

#include <time.h>

// localclient.c
int				create_unix_socket(void);
void			connect_local(int socket);
void			control_client(struct timespec *timeout);
void			send_client(char *data, int size);

#endif
