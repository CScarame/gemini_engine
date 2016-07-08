#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <stdlib.h>
#include <string>

#ifndef DEBUG
#define DEBUG 2
#endif

using namespace std;

void debug(string msg, int lvl=1)
{
	if (DEBUG >= lvl) cout << msg << endl;
}

#endif
