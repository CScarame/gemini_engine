// Header File for the timing default class
#ifndef TIMING_H
#define TIMING_H

#include "util.h"

#include <time.h>
#include <string.h>

const long NANOS_PER_SEC = 1000000000;

class Timing
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
	Timing();

	// Specified constructor. Param is fps
	Timing(int);

	~Timing() {}

	int run(void(*update)(), void(*render)());

	void setTPS(int);
	int getTPS();

	void get_time(struct timespec*);

	void set_mark(struct timespec);

	void add_mark(long nano);
	// Add nano nanoseconds to the mark's time

	long long mark(string);
	// opt gives the format of the return:
	// nano: in nanoseconds
	// micro: in microseconds
	// milli: in milliseconds
	// default: microseconds

	void quit(char);
	// Set q to true, ending run
};


#endif
