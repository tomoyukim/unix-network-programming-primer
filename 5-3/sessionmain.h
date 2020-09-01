#include <sys/types.h>
#include <netinet/in.h>

#define PORT           (in_port_t)50002
#define MAX_ATTENDANTS 5

extern void enter();
extern void sessionmain_init(int num, int maxfd);
extern void sessionmain_loop();