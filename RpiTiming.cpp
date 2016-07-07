// Default RpiTiming class

#include "RpiTiming.h"

const long NANOS_PER_SEC = 1000000000;

RpiTiming::RpiTiming()
{
	RpiTiming(60);
}

RpiTiming::RpiTiming(int set_tps)
{
	debug("Creating Timer");

	tps = set_tps;

	period = NANOS_PER_SEC / tps;

	num_ticks = 0;
	num_frames = 0;

	q = 0; // Quit variable. If 0, keep going

	get_time(&ct);
	mt.tv_sec = ct.tv_sec;
	mt.tv_nsec = ct.tv_nsec;

}

RpiTiming::~RpiTiming()
{
	debug("Cleaning up Timer");
}

int RpiTiming::run(void(*u)(), void(*r)())
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
	// Add try except for above loop?
	debug("Ended Timer Loop");

	return q; // Return reason for quitting
}

void RpiTiming::setTPS(int set_tps)
{
	debug("Setting TPS");
	tps = set_tps;
}

int RpiTiming::getTPS()
{
	return tps;
}

void RpiTiming::get_time(struct timespec *t)
{
	debug("Getting current time",3);
	clock_gettime(CLOCK_MONOTONIC, t);
}

void RpiTiming::set_mark(struct timespec t)
{
	debug("Setting mark",3);
	mt.tv_sec = t.tv_sec;
	mt.tv_nsec = t.tv_nsec;
}

void RpiTiming::add_mark(long nano)
{
	debug("Increasing mark",3);
	long tmp = mt.tv_nsec + nano;
	mt.tv_sec += tmp / NANOS_PER_SEC;
	mt.tv_nsec = tmp % NANOS_PER_SEC;
}

long long RpiTiming::mark()
{
	debug("Getting time since mark",3);
	struct timespec dt;

	get_time(&ct);

	dt.tv_sec = ct.tv_sec - mt.tv_sec;
	dt.tv_nsec = ct.tv_nsec - mt.tv_nsec;

	if (dt.tv_sec < 0) { // Edge case, just reset and try again
		debug("Got negative time when checking mark, could clock have looped?");
		set_mark(ct);
		return 0;
	}

	if (dt.tv_nsec < 0) {
		dt.tv_sec--;
		dt.tv_nsec += NANOS_PER_SEC;
	}

	long long diff = dt.tv_sec * NANOS_PER_SEC + dt.tv_nsec;

	return diff;
}

void RpiTiming::quit(char reason = 1)
{
	debug("Timer told to quit");
	q = reason;
}

void print(){
  debug("Test",0);
}

void blah(){}

int main(int argc, char* argv[])
{
  RpiTiming t(1);


  t.run(&print,&blah);
}
