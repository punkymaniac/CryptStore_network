#ifndef LOCALCLIENT_H
#define LOCALCLIENT_H

// localclient.c
int				create_unix_socket(void);
void			connect_local(int socket);
void			control_client(void);

#endif
