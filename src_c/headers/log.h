/*
  http://www.cs.virginia.edu/~jws9c/temple/307Fall2002/stdarg/
*/

#ifndef __LOG_H__
#define __LOG_H__

#include "config.h"

#include <stdarg.h>
#include <stdio.h>

#include <errno.h>
#include <stdlib.h>
#include <ctype.h>

class FilePrint{
private:
  FILE *log_file;
public:
  FilePrint(char *file){init_log(file);};

  void init_log(char *file);
  int log(char *fmt, ...);
};

#endif