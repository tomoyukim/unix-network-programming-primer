#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORT            (in_port_t)50002
#define HOSTNAME_LENGTH 64

extern void session_init();
extern void session_loop();