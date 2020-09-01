#include <stdio.h>
#include "sessionmain.h"
#include "mylib.h"

main(int argc, char *argv[]) {
  int num;
  int soc;
  int maxfd;

  // TODO: more flexible config
  if (argc != 2) {
    fprintf(stderr, "Usage: s attendants\n");
    exit(0);
  }

  if ((num = atoi(argv[1])) <= 0 || num > MAX_ATTENDANTS) {
    fprintf(stderr, "attendants limit=%d\n", MAX_ATTENDANTS);
    exit(1);
  }

  if ((soc = mserver_socket(PORT, num)) == -1) {
    fprintf(stderr, "cannot setup server\n");
    exit(0);
  }

  maxfd = mserver_maccept(soc, num, enter);

  sessionmain_init(num, maxfd);

  sessionmain_loop();
}