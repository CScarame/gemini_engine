// Header file for timing inputs
#ifndef _TIMING_H
#define _TIMING_H

#include <stdio.h>
#include <time.h>


// Get current time in sec and nanoseconds
void get_time(long* sec, long *nsec);

// Set the marker for the time passed
void set_mark(long sec, long nsec);

// Return the time passed since mark in nanoseconds
long get_mark(void);

#endif
