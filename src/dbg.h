#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// Debug command. Define NDEBUG to prevent debug

#ifdef NDEBUG
#define debug(M,...)
#else
#define debug(M,...) fprintf(stderr,"DEBUG: "); fprintf(stderr, M,##__VA_ARGS__);\
  fprintf(stderr,"\n")
#endif

// Put nice errno str

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

// Logging commands

#define log(M,...) fprintf(stderr,M,##__VA_ARGS__); fprintf(stderr,"\n")

#define log_err(M,...) \
  fprintf(stderr,"[ERR]  (%s:%d: errno: %s) ",__FILE__,__LINE__,clean_errno());\
  log(M,##__VA_ARGS__)

#define log_warn(M,...) \
  fprintf(stderr,"[WARN] (%s:%d: errno: %s) ",__FILE__,__LINE__,clean_errno());\
  log(M,##__VA_ARGS__)

#define log_info(M,...) \
  fprintf(stderr,"[INFO] (%s:%d) ",__FILE__,__LINE__);\
  log(M,##__VA_ARGS__)

#define check(A,M,...) if(!(A)) {log_err(M,##__VA_ARGS__);errno=0;goto error;}

#define check_mem(A) check((A), "Out of memory.")

char LOGFILE[] = "log.txt";

void set_stderr(char * logfile = LOGFILE,char * name)
{
  FILE *err;

  time_t ct;
  struct tm *lt;
  time(&ct);
  lt = localtime(&ct);

#ifdef DEBUG_CLEAN
  err = fopen(logfile, "w");
#else
  err = fopen(logfile, "a");
#endif

  char buffer[80];

  strftime(buffer, 80, "%x|%X", lt);

  fprintf(err,"\nLog from %s|%s\n", name, buffer);

  dup2(fileno(err),fileno(stderr));
  fclose(err);
}

