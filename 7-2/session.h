#include <netinet/in.h>

extern int session_seupclient(char *hostname, in_port_t port);
extern void session_init();
extern void session_loop();