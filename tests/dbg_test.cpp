/*
Output to dbg_test.log should be:
Log from ./dbg_test at ...|...
DEBUG: Testing debug
[ERR]  (dbg_test.cpp:11: errno: None) Param not 0: 1
[WARN] (dbg_test.cpp:19: errno: None) Second arg: None
[INFO] (dbg_test.cpp:25) Testing info: SUCCESS
[ERR]  (dbg_test.cpp:32: errno: None) Sentinel not 0
*/

#include "../src/dbg.h"

int test_debug()
{
  debug("Testing debug");
  return 0;
}

int test_check(int a)
{
  check(a==0,"Param not 0: %d",a);
  return 0;
error:
  return -1;
}

int test_warn(char* msg)
{
  log_warn("%s%s","Second arg: ",msg);
  return 0;
}

int test_info()
{
  log_info("Testing info: %s", "SUCCESS");
  return 0;
}

int test_sentinel(int a)
{
  if(a){
    sentinel("Sentinel not 0");
  }
  return 0;
error:
  return -1;
}

int all_tests(char* arg)
{
  check(test_debug() == 0, "Debug failed");
  check(test_check(0) == 0, "Check true failed");
  check(test_check(1) == -1, "Check false failed");
  check(test_warn(arg) == 0, "Warn failed");
  check(test_info() == 0, "Info failed");
  check(test_sentinel(0) == 0, "Sentinel false failed");
  check(test_sentinel(1) == -1, "Sentinel true failed");
  return 0;
error:
  return -1;
}

int main(int argc, char* argv[])
{
  char log[] = "dbg_test.log";
  set_stderr(log,argv[0]);

  char arg[] = "None";

  return all_tests((argc>1)?argv[1]:arg);
}
