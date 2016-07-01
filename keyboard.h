// Header file specifically for console/small_screen

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#ifndef DEBUG
#define DEBUG 0
#endif

#include <stdio.h>   // For printf
//#include <stdlib.h>
//#include <unistd.h>
#include <termios.h> // For various keyboard input config
#include <fcntl.h>  // For file protections
//#include <sys/ioctl.h>
#include <linux/kd.h> // For kb mode and other keyboard config
//#include <linux/keyboard.h>

// Enum for key inputs
typedef enum
{
  k_up,
  k_left,
  k_down,
  k_right,
  k_jump,
  k_stance,
  k_a,
  k_b,
  k_x,
  k_y,
  k_enter,
  k_esc,
  num_keys
} keyflag_t;

// Read key presses from the buffer and update keys
void tick();

// Updates keys variable. Down is a boolean.
void update_keys(short keycode, char down);

// Return value has a bit for each key above, 1 is down, 0 up
// LSB is k_up.
int get_keys();

// Returns 1 if k is down
char check_key(keyflag_t k);

// Called to begin keyboard input.
// Enables medium raw input (keycodes)
// Disables console input.
// Non-canonical inputs (No enter is needed, every press sends a code
// Disables repeat
void init();

// Returns keyboard to former settings 
void cleanup();

#endif // _KEYBOARD_H
