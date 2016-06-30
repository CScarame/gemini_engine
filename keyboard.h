// Keys can be adjusted. Once the keyflags are read,
//    UP = KEYFLAGS | K_UP
//    This is zero if the up key isn't pressed
//    This is non-zero if up key is pressed

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdio.h>    // printf
#include <stdlib.h>   // malloc
#include <linux/kd.h> // Keyboard

// A list of all key inputs. These are
//  used to make flags for the keys int
enum KEYFLAG {
  K_UP    = 0x000;
  K_LEFT  = 0x001;
  K_DOWN  = 0x002;
  K_RIGHT = 0x004;
  K_JUMP  = 0x008;
  K_STANCE= 0x010;
  K_A     = 0x020;
  K_B     = 0x040;
  K_X     = 0x080;
  K_Y     = 0x100;
  K_ESC   = 0x200;
  K_ENTER = 0x400;
};

// Function to return the flags for
//  which keys are down.
int get_keys();

// Refresh keyflags
void tick();

// Change a key mapping. The next key to be pressed
//  becomes mapped to that flag.
void remap(KEYFLAG kf);

#endif
