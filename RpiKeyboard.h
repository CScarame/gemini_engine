#ifndef RPI_KEYBOARD_H
#define RPI_KEYBOARD_H

#include "Keyboard.h"

#include <stdio.h>   // For printf
#include <stdlib.h>
#include <unistd.h>
#include <termios.h> // For various keyboard input config
#include <fcntl.h>  // For file protections
#include <sys/ioctl.h>
#include <linux/kd.h> // For kb mode and other keyboard config
#include <linux/keyboard.h>

class RpiKeyboard
{
	int fd = 0; // The file descriptor for the text input
	int oldkbmode = K_RAW;  // Used to hold the old keyboard mode, to reset
	struct termios orig_kb; // Old termios keyboard data, to reset
	struct termios new_kb;  // New termios keyboard data for use in this program

	char active = 0;  // Is 1 after init has been called and before cleanup

	int keys = 0;  // Variable that holds key status

	int remap = -1; // If this isn't -1, a key is being remapped

	void update_keys(short, char);

public:
	RpiKeyboard();

	~RpiKeyboard();

	void update();

	int get_keys();

	char check_key(keyflag_t);

	int remap_key(keyflag_t);
};

#endif