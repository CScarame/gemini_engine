// TODO: solve for when clock loops
// Solve for negative mark

#include "timing.h"

#include <stdio.h>

const long NANOS_PER_SEC = 1000000000;

struct timespec ct;
struct timespec mt;

// Returns current time in nanosecs
void get_time(long* sec, long* nano)
{
  clock_gettime(CLOCK_MONOTONIC, &ct);
  *sec = ct.tv_sec;
  *nano = ct.tv_nsec;
}

void set_mark(long sec, long nano)
{
  mt.tv_sec = sec;
  mt.tv_nsec = nano;
}

long mark(void)
{
  clock_gettime(CLOCK_MONOTONIC, &ct);
  long d_sec = (long) ct.tv_sec - mt.tv_sec;
  long d_nano = (long) ct.tv_nsec - mt.tv_nsec;

  while(d_nano < 0){
    d_sec++;
    d_nano = d_nano - NANOS_PER_SEC;
  }

  long tot_nano = d_sec*NANOS_PER_SEC + d_nano;
  return tot_nano;
}

int main(){
  long s;
  long n;
  get_time(&s,&n);
  set_mark(s,n);
  int i;
  for(i = 0; i < 10; i++){
    while(mark() < NANOS_PER_SEC);
    printf("%d\n",mark());
    set_mark(s++,n);

  }
}
