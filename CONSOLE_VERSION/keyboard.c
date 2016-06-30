// Header file specifically for console/small_screen

#include "keyboard.h"

int fd = 0;
int oldkbmode = K_RAW;
struct termios orig_kb;
struct termios new_kb;

unsigned char buf;

char active = 0;

// If init has been called, then finds the status of keys
void tick()
{
  if(!active) return;

  if(DEBUG >= 2) printf("tick keyboard");

  unsigned char down;
  unsigned char keycode;
  while(read(fd,&buf,1) >= 0) {
    down = buf & 0x80;
    keycode = buf & 0x7F;
    update_keys(keycode,down);
  }
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

  if(DEBUG >= 1) printf("Successfully initialized keyboard");

  active = 1; // Shows the keyboard is active

  atexit(cleanup);
}

void cleanup()
{
  ioctl(fd,KDSKBMODE,oldkbmode);

  // Perform a sys call to restore defaults
  system("sudo kbdrate -s");


  tcsetattr(fd,0,&orig_kb);
  close(fd);

  if(DEBUG >= 1) printf("Cleaned up keyboard");
  active = 0; // Shows the keyboard is inactive
}

int main() // For Testing only
{
  init();
  while(buf!=1) {
    printf(".\n");
    sleep(1);
    // Wait until a key is pressed/released
    int read_len;
    while( read(fd,&buf,1) >= 0){
     printf("%08x\n",buf);
    }
  }
  cleanup();
  return 0;
}
