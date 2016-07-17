// Header File for the timing default class
#ifndef RPI_TIMING_H
#define RPI_TIMING_H

#include "Timing.h"

#include <time.h>
#include <string.h>

class RpiTiming
{
	void(*update)(void);
	void(*render)(void);

	int tps; // ticks per second

	long period; // ns between each tick

	struct timespec ct;
	struct timespec mt;

	unsigned long num_ticks;
	unsigned long num_frames;

	char q;

public:
	// Default constructor
	RpiTiming();

	// Specified constructor. Param is fps
	RpiTiming(int);

	~RpiTiming();

	int run(void(*update)(), void(*render)());
	// Runs the timing calling update the number of tps per seconds

	void setTPS(int);
	int getTPS();
	// Can set TPS mid game (probablyy bad?)

	void get_time(struct timespec*);
	// Get the current time in a timespec

	void set_mark(struct timespec);

	void add_mark(long nano);
	// Add nano nanoseconds to the mark's time

	long mark();
	// Returns nanoseconds since mt


	void quit(char);
	// Set q to true, ending run
};


#endif
