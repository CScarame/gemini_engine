#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>

#define DEBUG 0

void debug(char* msg, int lvl=1)
{
	if (DEBUG >= lvl) puts(msg);
}

#endif
