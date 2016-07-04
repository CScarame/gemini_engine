
#ifndef SMALL_RPI_SCREEN_H
#define SMALL_RPI_SCREEN_H

#include "util.h"
#include "Screen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/mman.h>

class SmallRpiScreen : Screen
{
	const char* FRAMEBUFFER;
	const char* CONSOLE;
	const int PAGES = 1;

	int fbfd = 0; // Framebuffer file descriptor
	int kbfd = 0; // Keyboard file descriptor

	struct fb_fix_screeninfo finfo;  // Struct for fixed screen info
	struct fb_var_screeninfo vinfo;  // For variable screen info
	struct fb_var_screeninfo orig_vinfo; // To store original vinfo for reset

	long len_fb = 0; // Length of framebuffer memory
	char *fbp = 0; // Framebuffer pointer

	char* fbpage = 0; // Pointer to the page memory location, because
					  // The small screen can't load in any virtual space
	int page = 0;  // Current page being written to (always 0 for sm screen)

public:
	// Default Constructor
	SmallRpiScreen();

	// Specific Constructor
	SmallRpiScreen(int*, int*, int*);

	// Destructor
	~SmallRpiScreen();

	void draw_pixel(int, int, int);

	void clear_screen(int);

	void switch_page();
	
};

#endif
