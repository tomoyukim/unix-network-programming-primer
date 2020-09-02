#include <stdio.h>
#include <sys/types.h>

#define MAX_ATTENDANTS 5
#define BUF_LEN        80

static char buf[BUF_LEN];
static fd_set mask;
static int width;
static int attendants;

typedef struct {
  int fd;
  char name[16];
} ATTENDANT;

static ATTENDANT p[MAX_ATTENDANTS];

static void send_all(int i, int n);
static void ending();

void enter(int i, int fd) {
  int len;
  static char *mesg1 = "Type your name.\n";
  static char *mesg2 = "Wait.\n";

  p[i].fd = fd;
  memset(p[i].name, 0, 16);
  write(fd, mesg1, strlen(mesg1));
  len = read(fd, p[i].name, 16);
  sprintf(p[i].name + len -1, "  -->  ");
  write(fd, mesg2, strlen(mesg2));
}

void sessionmain_init(int num, int maxfd) {
  int i;
  static char *mesg = "Communication Ready.\n";
  attendants = num;

  width = maxfd + 1;
  FD_ZERO(&mask);
  FD_SET(0, &mask);
  for (i = 0; i < num; i++) {
    FD_SET(p[i].fd, &mask);
  }

  for (i = 0; i < num; i++) {
    write(p[i].fd, mesg, strlen(mesg));
  }
}

void sessionmain_loop() {
  fd_set readOk;
  int i;

  while (1) {
    readOk = mask;
    select(width, (fd_set *)&readOk, NULL, NULL, NULL);

    // TODO better ending session
    if (FD_ISSET(0, &readOk)) {
      ending();
    }

    for (i = 0; i < attendants; i++) {
      if (FD_ISSET(p[i].fd, &readOk)) {
        int n;
        n = read(p[i].fd, buf, BUF_LEN);
        send_all(i, n);
      }
    }
  }
}

static void ending() {
  int i;
  for (i = 0; i < attendants; i++) {
    write(p[i].fd, "q", 1);
  }
  for (i = 0; i < attendants; i++) {
    close(p[i].fd);
  }
  exit(0);
}

static void send_all(int i, int n) {
  int j;
  for (j = 0; j < attendants; j++) {
    write(p[j].fd, p[i].name, strlen(p[i].name));
    write(p[j].fd, buf, n);
  }
}