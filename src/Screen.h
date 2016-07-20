#ifndef SCREEN_H
#define SCREEN_H

#include "dbg.h"

class Screen
{
public:
	// Default constructor
	Screen() {}

	// Specified constructor
	Screen(int*, int*, int*) {}

	// Destructor
	~Screen() {}

	virtual void draw_pixel(int, int, int) = 0;
	virtual void draw_rect(int,int,int,int,int) = 0;
	virtual void clear_screen(int) = 0;
	virtual void switch_page() = 0;
};



#endif
