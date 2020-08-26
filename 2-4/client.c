#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT    (in_port_t)50000
#define BUF_LEN 512

main() {
  struct hostent *server_ent;
  struct sockaddr_in server;
  int soc;
  char hostname[] = "stretch";
  char buf[BUF_LEN];

  if((server_ent = gethostbyname(hostname)) == NULL ) {
    perror("gethostbyname");
    exit(1);
  }

  printf("h_name: %s\n", server_ent->h_name);
  printf("h_aliases[0]: %s\n", server_ent->h_aliases[0]);
  printf("h_addrtype:%d\n", server_ent->h_addrtype);
  printf("h_length:%d\n", server_ent->h_length);
  printf("h_addr:%s\n", server_ent->h_addr_list[0]);

  memset((char *)&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  memcpy((char *)&server.sin_addr, server_ent->h_addr_list[0], server_ent->h_length);

  if((soc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  if(connect(soc, (struct sockadr *)&server, sizeof(server)) == -1) {
    perror("connect");
    exit(1);
  }

  write(1, "Wait\n", 5);

  do {
    int n;

    write(1, "<<Wait message..\n", 18);
    n = read(soc, buf, BUF_LEN);
    write(1, buf, n);

    write(1, ">>Please input message:\n", 24);
    n = read(0, buf, BUF_LEN);
    write(soc, buf, n);
  } while (strncmp(buf, "quit", 4) != 0);

  close(soc);
}
