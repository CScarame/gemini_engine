
#ifndef SMALL_RPI_SCREEN_H
#define SMALL_RPI_SCREEN_H

#include "util.h"
#include "Screen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/mman.h>

class SmallRpiScreen : Screen
{
	const char* FRAMEBUFFER;
	const char* CONSOLE;

  int PAGES;

	int fbfd; // Framebuffer file descriptor
	int kbfd; // Keyboard file descriptor

	struct fb_fix_screeninfo finfo;  // Struct for fixed screen info
	struct fb_var_screeninfo vinfo;  // For variable screen info
	struct fb_var_screeninfo orig_vinfo; // To store original vinfo for reset

	long len_fb; // Length of framebuffer memory
	char *fbp; // Framebuffer pointer

	char* fbpage; // Pointer to the page memory location, because
					  // The small screen can't load in any virtual space
	int page;  // Current page being written to (always 0 for sm screen)

public:
	// Default Constructor
	SmallRpiScreen();

	// Specific Constructor
	SmallRpiScreen(int*, int*, int*);

	// Destructor
	~SmallRpiScreen();

	void draw_pixel(int, int, int);

	void draw_rect(int,int,int,int,int);

	void clear_screen(int);

	void switch_page();

	int init(int*,int*,int*);
};

#endif
