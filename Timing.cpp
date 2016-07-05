// Default Timing class

#include "Timing.h"

Timing::Timing()
{
	Timing(60);
}

Timing::Timing(int set_tps)
{
	debug("Creating Timer");

	tps = set_tps;

	period = NANOS_PER_SEC / tps;

	num_ticks = 0;
	num_frames = 0;

	quit = 0;

	get_time(&ct);
	mt.tv_sec = ct.tv_sec;
	mt.tv_nsec = ct.tv_nsec;

}

Timing::~Timing()
{
	debug("Cleaning up Timer");
}

int Timing::run(void(*u)(), void(*r)())
{
	debug("Running Timer");

	update = u;
	render = r;

	get_time(&ct);
	mt.tv_sec = ct.tv_sec;
	mt.tv_nsec = ct.tv_nsec;

	while (!q)
	{
		while (mark() > period) {
			update();
			num_ticks++;
			add_mark(period);
		}
		render();
		num_frames++;
		// Optionally, check how much time remains and sleep
	}

	return q; // Return reason for quitting
}

void Timing::setTPS(int set_tps)
{
	tps = set_tps;
}

int Timing::getTPS()
{
	return tps;
}

void Timing::get_time(struct timespec *t)
{
	clock_gettime(CLOCK_MONOTONIC, t);
}

void Timing::set_mark(struct timespec t)
{
	mt.tv_sec = t.tv_sec;
	mt.tv_nsec = t.tv_nsec;
}

void Timing::add_mark(long nano)
{
	long tmp = mt.tv_nsec + nano;
	mt.tv_sec += tmp / NANOS_PER_SEC;
	mt.tv_nsec = tmp % NANOS_PER_SEC;
}

long long Timing::mark(string opt = "nano")
{
	struct timespec dt;

	get_time(&ct);

	dt.tv_sec = ct.tv_sec - mt.tv_sec;
	dt.tv_nsec = ct.tv_nsec - mt.tv_nsec;

	if (dt.tv_sec < 0) { // Edge case, just reset and try again
		set_mark(ct);
		return 0;
	}

	if (dt.tv_nsec < 0) {
		dt.tv_sec--;
		dt.tv_nsec += NANOS_PER_SEC;
	}

	long long diff = dt.tv_sec * NANOS_PER_SEC + dt.tv_nsec;

	switch (opt)
	{
	case "micro":
		return diff / 1000;
	case "milli":
		return diff / 1000000;
	default:
		return diff;
	}
}

void Timing::quit(char reason = 1)
{
	q = reason;
}
