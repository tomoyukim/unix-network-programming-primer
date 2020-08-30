#include "goban.h"

main() {
  int soc;
  char my_stone = 'o';
  char peer_stone = 'x';

  if ((soc = setup_server(PORT)) == -1) {
    exit(1);
  }

  goban_init(soc, my_stone, peer_stone);

  while(1) {
    goban_show();
    printf("\033[0J");
    printf("Wait.\n");
    if (goban_peer_turn() == -1) {
      break;
    }

    goban_show();
    printf("\033[0J");
    printf("Go ahead.\n");
    if (goban_my_turn() == -1) {
      break;
    }
  }

  goban_destroy();
}
