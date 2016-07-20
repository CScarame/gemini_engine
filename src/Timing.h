// Header File for the timing default class
#ifndef TIMING_H
#define TIMING_H

#include "dbg.h"


class Timing
{
public:
	// Default constructor
	Timing() {};

	// Specified constructor. Param is fps
	Timing(int) {};

	~Timing() {};

	virtual int run(void(*update)(), void(*render)()) = 0;
	// Runs the timing calling update the number of tps per seconds

	virtual void setTPS(int) = 0;
	virtual int getTPS() = 0;
	// Can set TPS mid game (probablyy bad?)

	void get_time(struct timespec*);
	// Get the current time in a timespec

	void set_mark(struct timespec);

	void add_mark(long nano);
	// Add nano nanoseconds to the mark's time

	long mark();
	// Returns nanoseconds since mt


	virtual void quit(char) = 0;
	// Set q to true, ending run
};


#endif
