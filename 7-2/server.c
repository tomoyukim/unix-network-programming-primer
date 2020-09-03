#include <stdio.h>
#include "mtalk2.h"
#include "sessionmain.h"

main() {
  if (sessionmain_init(PORT) == -1) {
    fprintf(stderr, "cannot setup server\n");
    exit(1);
  }

  sessionmain_loop();
}