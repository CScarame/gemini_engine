#ifndef SCREEN_H
#define SCREEN_H

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
// Init may attempt to use the values in
//  width, height, bpp to set the values of
//  of the screen. It may just go to default
//  values. The final values are returned in
//  int these variables.
//
// Returns 0 for successful initialization
//  and a positive number for an error.
int init(int* width, int* height, char* bpp);

// Closes the screen and cleans up after it.
// Call this last. Returns 0 for success.
int cleanup();

#endif
