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

void init();

void cleanup();

#endif // _KEYBOARD_H
