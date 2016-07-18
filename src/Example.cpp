#include "RpiScreen.cpp"
#include "RpiKeyboard.cpp"
#include "RpiTiming.cpp"

const int WIDTH = 320;
const int HEIGHT = 240;
const int SPEED = 2;
const int SPEED2 = 1;

int s;

int x;
int y;

int x2;
int y2;

int dx;
int dy;

int dx2;
int dy2;

RpiScreen *scPtr;
RpiKeyboard *kbPtr;
RpiTiming *tmPtr;

void update_pos()
{
  int ox = x;
  x = x + dx;
  if(x < 0 || x + s >= WIDTH){
    x = ox;
  }

  int oy = y;
  y = y + dy;
  if(y < 0 || y + s >= HEIGHT){
    y = oy;
  }
  int ox2 = x2;
  x2 = x2 + dx2;
  if(x2 < 0 || x2 + s >= WIDTH){
    x2 = ox2;
  }

  int oy2 = y2;
  y2 = y2 + dy2;
  if(y2 < 0 || y2 + s >= HEIGHT){
    y2 = oy2;
  }
}

void update()
{
  kbPtr->update();

  dy = 0;
  if(kbPtr->check_key(k_up) && !kbPtr->check_key(k_down)){
    dy = -1 * SPEED;
  }if(kbPtr->check_key(k_down) && !kbPtr->check_key(k_up)){
    dy = 1 * SPEED;
  }
  dx = 0;
  if(kbPtr->check_key(k_left) && !kbPtr->check_key(k_right)){
    dx = -1 * SPEED;
  }if(kbPtr->check_key(k_right) && !kbPtr->check_key(k_left)){
    dx = 1 * SPEED;
  }
  dy2 = 0;
  if(kbPtr->check_key(k_up) && !kbPtr->check_key(k_down)){
    dy2 = -1 * SPEED2;
  }if(kbPtr->check_key(k_down) && !kbPtr->check_key(k_up)){
    dy2 = 1 * SPEED2;
  }
  dx2 = 0;
  if(kbPtr->check_key(k_left) && !kbPtr->check_key(k_right)){
    dx2 = -1 * SPEED2;
  }if(kbPtr->check_key(k_right) && !kbPtr->check_key(k_left)){
    dx2 = 1 * SPEED2;
  }

  update_pos();

  if(kbPtr->check_key(k_esc)){
    tmPtr->quit(1);
  }
}

void render()
{
  scPtr->clear_screen(0);
  scPtr->draw_rect(x,y,s,s,0x001F);
  scPtr->draw_rect(x2,y2,s,s,0xF800);
  scPtr->switch_page();
}

int main(){

  printf("%d\n",k_up);

  x = 0;
  y = 0;
  x2= 25;
  y2= 0;
  s = 25;
  dx = 0;
  dy = 0;
  dx2= 0;
  dy2= 0;
  int w = 320, h = 240, bpp = 16;
  RpiScreen sc(&w,&h,&bpp);
  scPtr = &sc;
  RpiKeyboard kb;
  kbPtr = &kb;
  RpiTiming tm(60);
  tmPtr = &tm;

  tmPtr->run(update,render);
}
