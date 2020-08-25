#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT     (in_port_t)50000
#define BUF_LEN  512

main() {
  struct sockaddr_in me;
  int soc_waiting;
  int soc;
  char buf[BUF_LEN];

  memset((char *)&me, 0, sizeof(me));
  me.sin_family = AF_INET;
  me.sin_addr.s_addr = htonl(INADDR_ANY);
  me.sin_port = htons(PORT);

  // create stream socket by IPv4
  if ((soc_waiting = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  // bind address of me with socket
  if (bind(soc_waiting, (struct sockaddr *)&me, sizeof(me)) == -1) {
    perror("bind");
    exit(1);
  }

  listen(soc_waiting, 1);

  // block until connection request
  soc = accept(soc_waiting, NULL, NULL);

  close(soc_waiting);

  write(1, "Go ahead!\n", 10);

  do {
    int n;
    n = read(0, buf, BUF_LEN);
    write(soc, buf, n);
    n = read(soc, buf, BUF_LEN);
    write(1, buf, n);
  } while (strncmp(buf, "quit", 4) != 0);

  close(soc);
}