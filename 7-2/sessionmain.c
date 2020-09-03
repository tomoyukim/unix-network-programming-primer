#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>

#include "mtalk2.h"

typedef struct {
  int flag;
  char name[12];
  struct sockaddr_in address;
} TALKER;

static int soc;
static char buf[BUF_LEN];
static TALKER talker[MAX_ATTENDANTS];

static struct sockaddr_in me;
static struct sockaddr_in from;
static int fromlen;
static int len;

static void do_login();
static void do_logout();
static int find_free_slot();
static void distribute();
static void ending();

int sessionmain_init(in_port_t port) {
  int i;

  memset((char *)&me, 0, sizeof(me));
  me.sin_family = AF_INET;
  me.sin_addr.s_addr = htonl(INADDR_ANY);
  me.sin_port = htons(port);

  if ((soc = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    return -1;
  }

  if (bind(soc, (struct sockaddr *)&me, sizeof(me)) == -1) {
    perror("bind");
    return -1;
  }
  fprintf(stderr, "successfully bound.\n");

  for (i = 0; i < MAX_ATTENDANTS; i++) {
    talker[i].flag = FREE;
  }
  signal(SIGINT, ending);
}

void sessionmain_loop() {
  while(1) {
    fromlen = sizeof(from);
    len = recvfrom(soc, buf, BUF_LEN, 0, &from, &fromlen);
    switch (buf[0]) {
      case LOGIN:
        do_login();
        break;
      case LOGOUT:
        do_logout();
        break;
      case DATA:
        distribute();
        break;
      default:
        break;
      }
  }
}

static void do_login() {
  int slot;

  if ((slot = find_free_slot()) == -1) {
    buf[0] = CANNOT_LOGIN;
    sendto(soc, buf, 2, 0, &from, sizeof(from));
    return;
  }

  talker[slot].flag = IN_USE;
  strcpy(talker[slot].name, buf + 1);
  talker[slot].address = from;

  printf("talker[%d] :%s logged in.\n", slot, talker[slot].name);

  buf[0] = LOGIN_OK;
  sprintf(buf + 1, "%02d\n", slot);
  sendto(soc, buf, 4, 0, &from, sizeof(from));
}

static void do_logout() {
  int slot;

  sscanf(buf + 1, "%02d", &slot);
  talker[slot].flag = FREE;

  printf("talker[%d] :%s logged out.\n", slot, talker[slot].name);
}

static int find_free_slot() {
  int slot;

  for (slot = 0; slot < MAX_ATTENDANTS; slot++) {
    if (talker[slot].flag == FREE) {
      return slot;
    }
  }
  return -1;
}

static void distribute() {
  int slot;

  buf[0] = DATA;
  for (slot = 0; slot < MAX_ATTENDANTS; slot++) {
    if (talker[slot].flag == FREE) {
      continue;
    }
    printf("talker[%d] :%s says %s\n", slot, talker[slot].name, buf);
    sendto(soc, buf, len, 0, &talker[slot].address, sizeof(from));
  }
}

static void ending() {
  int slot;

  buf[0] = END;
  for (slot = 0; slot < MAX_ATTENDANTS; slot++) {
    if (talker[slot].flag == FREE) {
      continue;
    }
    sendto(soc, buf, 1, 0, &talker[slot].address, sizeof(from));
  }
  exit(0);
}