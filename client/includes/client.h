#ifndef CLIENT_H
#define CLIENT_H

#define SIZE_BUFFER 1024

// client.c
void			run_shell(void);
int				connect_daemon(void);

// command.h
void			run_cmd(char *cmd, char **arg);

#endif
