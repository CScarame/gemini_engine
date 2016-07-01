 // Header file specifically for console/small_screen
//#define DEBUG 1
#include "keyboard.h"

int fd = 0;
int oldkbmode = K_RAW;
struct termios orig_kb;
struct termios new_kb;

unsigned char buf;

char active = 0;

int keys = 0;

unsigned char keycodes[] = {
  17,
  30,
  31,
  32,
  57,
  42,
  36,
  37,
  38,
  39,
  28,
  1
};

// If init has been called, then finds the status of keys
void tick()
{
  if(!active) return;

  if(DEBUG >= 2) printf("tick keyboard");

  char down;
  unsigned char keycode;
  while(read(fd,&buf,1) >= 0) {
    down = !(buf & 0x80); // bit is 0 for down, 1 for up
    keycode = buf & 0x7F;    // The rest is put into keycode
    update_keys(keycode,down);
  }
}

void update_keys(short keycode, char down)
{
  if(DEBUG >= 1) {
    printf("Update: %d ",keycode);
    if(down){
      printf("down\n");
    }else{
      printf("up\n");
    }
  }

  int i;
  for(i=0;i<num_keys;i++){
    if(keycode == keycodes[i]){
      if(down){
        keys |= (1<<i); // Mask the bit on
      } else{
        keys &= ~(1<<i);// Mask the bit off
      }
    }
  }
}

int get_keys(){
  return keys;
}

char test_key(keyflag_t k){
  char result = (keys & (1<<k))?1:0;
}

void init()
{

  // Open and config the keyboard */
  fd = open("/dev/tty", O_RDONLY | O_NDELAY);
  tcgetattr(fd, &orig_kb);
  tcgetattr(fd, &new_kb);
  new_kb.c_lflag &= ~(ECHO|ICANON|ISIG);
  new_kb.c_iflag = IXOFF;
  new_kb.c_cc[VTIME] = 0;
  tcsetattr(fd, TCSAFLUSH, &new_kb);

  // Set med raw
  ioctl(fd,KDGKBMODE,&oldkbmode);
  ioctl(fd,KDSKBMODE,K_MEDIUMRAW);

  // Perform a sys call to prevent repeats
  system("sudo kbdrate -r 2.0 -d 1000000 -s"); 

  if(DEBUG >= 1) printf("Successfully initialized keyboard\n");

  active = 1; // Shows the keyboard is active
}

void cleanup()
{
  ioctl(fd,KDSKBMODE,oldkbmode);

  // Perform a sys call to restore defaults
  system("sudo kbdrate -s");


  tcsetattr(fd,0,&orig_kb);
  close(fd);

  if(DEBUG >= 1) printf("Cleaned up keyboard\n");
  active = 0; // Shows the keyboard is inactive
}

int main() // For Testing only
{
  init();
  keys = 0;
  int i;
  while(!(test_key(k_esc))){
    tick();
    printf("%x\n",keys);
    //sleep(1);
  }
  cleanup();
  return 0;
}
