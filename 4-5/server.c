#include "session.h"
#include "tag.h"

main() {
  int soc;

  if ((soc = setup_server(PORT)) == -1) {
    exit(1);
  }

  session_init(soc, 'o', 1, 1, 'x', 10, 10);

  session_loop();
}