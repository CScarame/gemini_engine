 // Header file specifically for console/small_screen
//#define DEBUG 1
#include "keyboard.h"

int fd = 0; // The file descriptor for the text input
int oldkbmode = K_RAW;  // Used to hold the old keyboard mode, to reset
struct termios orig_kb; // Old termios keyboard data, to reset
struct termios new_kb;  // New termios keyboard data for use in this program

char active = 0;  // Is 1 after init has been called and before cleanup

int keys = 0;  // Variable that holds key status

int remap = -1; // If this isn't -1, a key is being remapped

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

// Updates the status of the keys variable.
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

  if(remap == -1){
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
  }else {
    if(down){
      keycodes[remap] = keycode;
      remap = -1;
    }
  }
}


// If init has been called, then finds the status of keys
void tick()
{
  if(!active) return;

  if(DEBUG >= 2) printf("tick keyboard");

  char down; // Used to hold whether the key was moved to up or down
  unsigned char keycode; // Used to hold the keycode of the presed key

  unsigned char buf;  // Characters are read one at a time to this buffer.

  while(read(fd,&buf,1) >= 0) {
    down = !(buf & 0x80); // bit is 0 for down, 1 for up
    keycode = buf & 0x7F;    // The rest is put into keycode
    update_keys(keycode,down);
  }
}

int get_keys(){
  return keys;
}

char test_key(keyflag_t k){
  char result = (keys & (1<<k))?1:0;
}

// After this function is called, the next key to be pressed
// Becomes the mapping for the parameter keyflag
int remap_key(keyflag_t k){
  remap = (int)k;
  int temp = remap;

  // The following is used to freeze everything while the key is being remapped
  while(remap != -1){
    tick();
  }
  return (int)keycodes[temp];
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

  remap_key(k_up);
  printf("%d\n",keycodes[0]);

  int i;
  while(!(test_key(k_esc))){
    tick();
    printf("%x\n",keys);
    //sleep(1);
  }
  cleanup();
  return 0;
}
