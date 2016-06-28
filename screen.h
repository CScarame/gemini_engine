#ifndef GEMINI_SCREEN_H
#define GEMINI_SCREEN_H

#include <stdio.h>    // printf
#include <stdlib.h>   // malloc
#include <string.h>   // memset memcpy
#include <fcntl.h>    // Open files
#include <linux/fb.h> // Framebuffer
#include <linux/kd.h> // Keyboard
#include <sys/mman.h> // Shared mapping


// Turn the pixel at (x,y) on the current
// page to the color c
// x and y will be bounded by xres and yres
// color c is in 32 bpp format. It will be
// formatted to the correct bpp
void put_pixel(int x, int y, int c);

// Blank the entire page to the color c
void clear_screen(int c);

// Switch to next page
void draw();

// Starts the screen. Call this first.
int init(int* width, int* height, char* bpp);

#endif
