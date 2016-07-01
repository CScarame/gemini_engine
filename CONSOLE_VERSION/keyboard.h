// Header file specifically for console/small_screen

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#ifndef DEBUG
#define DEBUG 0
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/kd.h>
#include <linux/keyboard.h>

typedef enum
{
  k_up,
  k_left,
  k_down,
  k_right,
  k_rump,
  k_stance,
  k_a,
  k_b,
  k_x,
  k_y,
  k_enter,
  k_esc,
  num_keys
} keyflag_t;

void tick();

void update_keys(short keycode, char down);

int get_keys();

char check_key(keyflag_t k);

void init();

void cleanup();

#endif // _KEYBOARD_H
