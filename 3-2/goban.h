#include <sys/types.h>
#include "mylib.h"

#define PORT            (in_port_t)50000
#define HOSTNAME_LENGTH 64

extern void goban_init(int soc, char my_stone, char peer_stone);
extern void goban_show_plane();
extern int goban_peer_turn();
extern int goban_my_turn();