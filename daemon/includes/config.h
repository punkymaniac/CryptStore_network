#ifndef CONFIG_H
#define CONFIG_H

#define DEFAULT_LISTEN_PORT 42680
#define DEFAULT_HOST_ADDRESS "127.0.0.1"

struct conf
{
	unsigned short		listen_port;
	char							host_address[16];
};

// main.c
struct conf					*get_config(void);

#endif
