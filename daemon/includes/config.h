#ifndef CONFIG_H
#define CONFIG_H

struct conf
{
	unsigned short		listen_port;
};

#define DEFAULT_LISTEN_PORT 42680

// main.c
struct conf					*get_config(void);

#endif
