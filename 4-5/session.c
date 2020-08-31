#include <sys/types.h>
#include <curses.h>
#include <signal.h>

#define BUF_LEN 20
#define MIN_X   1
#define MIN_Y   2  // +1 life display space
#define MAX_X   60
#define MAX_Y   21 // +1 life display space

#define NORTH  'k'
#define SOUTH  'j'
#define EAST   'l'
#define WEST   'h'
#define QUIT   'q'

static int session_soc;
static fd_set mask;
static int width;
static char my_char, peer_char;

typedef struct {
  int x, y;
  char look;
  int life;
  int ldx, ldy; // life display pos
} PLAYER;

static PLAYER me, peer;

static char buf[BUF_LEN];

static WINDOW *win;

static void hide(PLAYER *who);
static void show(PLAYER *who);
static int update(PLAYER *who, int c);
static int interpret(PLAYER *who);
static void die();

static void show_life(PLAYER *who);

// TODO: error handling and return
void session_init(int soc, char mc, int mx, int my, char pc, int px, int py) {
  session_soc = soc;
  width = soc + 1;
  FD_ZERO(&mask);
  FD_SET(0, &mask);
  FD_SET(session_soc, &mask);
  me.look = mc;
  peer.look = pc;

  me.x = mx;
  me.y = my;
  me.life = 100;
  me.ldx = 1;
  me.ldy = 1;
  peer.x = px;
  peer.y = py;
  peer.life = 100;
  peer.ldx = 1;
  peer.ldy = 10;

  initscr();
  signal(SIGINT, die);

  win = newwin(MAX_Y + 2, MAX_X + 2, 0, 0);
  box(win, '|', '-');

  cbreak();
  noecho();
}

void session_loop() {
  int c;
  int flag;
  fd_set readOk;

  show(&me);

  flag = 1;

  while (1) {
    readOk = mask;
    select(width, (fd_set *)&readOk, NULL, NULL, NULL);

    if (FD_ISSET(0, &readOk)) {
      c = getchar();
      hide(&me);
      flag = update(&me, c);
      show(&me);
      write(session_soc, buf, BUF_LEN);
      if (flag == 0) {
        break;
      }
    }

    if (FD_ISSET(session_soc, &readOk)) {
      read(session_soc, buf, BUF_LEN);
      hide(&peer);
      flag = interpret(&peer);
      show(&peer);
      if (flag == 0) {
        break;
      }
    }
  }
  die();
}

static void show(PLAYER *who) {
  show_life(who);
  wmove(win, who->y, who->x);
  waddch(win, who->look);
  wmove(win, who->y, who->x);
  wrefresh(win);
}

static void hide(PLAYER *who) {
  wmove(win, who->y, who->x);
  waddch(win, ' ');
}

static int update(PLAYER *who, int c) {
  switch(c) {
    case WEST:
      if (who->x > MIN_X) {
        who->x--;
        who->life--;
      } else {
        who->life -= 10;
      }
      break;
    case SOUTH:
      if (who->y < MAX_Y) {
        who->y++;
        who->life--;
      } else {
        who->life -= 10;
      }
      break;
    case NORTH:
      if (who->y > MIN_Y) {
        who->y--;
        who->life--;
      } else {
        who->life -= 10;
      }
      break;
    case EAST:
      if (who->x < MAX_X) {
        who->x++;
        who->life--;
      } else {
        who->life -= 10;
      }
      break;
    case QUIT:
      buf[0] = QUIT;
      return 0;
  }

  if (who->life < 1) {
    // game end
    buf[0] = QUIT;
    return 0;
  }

  sprintf(buf, "%d %d %d\n", who->x, who->y, who->life);
  return 1;
}

static int interpret(PLAYER *who) {
  if (buf[0] == 'q') {
    return 0;
  }
  sscanf(buf, "%d %d %d", &who->x, &who->y, &who->life);
  return 1;
}

static void die() {
  endwin();
  exit(0);
}

static void show_life(PLAYER *who) {
  char label[6];
  snprintf(label, 6, "%c:%03d", who->look, who->life);

  wmove(win, who->ldx, who->ldy);
  waddstr(win, label);
  wmove(win, who->ldx, who->ldy);
}
