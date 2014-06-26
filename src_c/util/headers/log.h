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

#include <string>

class FilePrint{
private:
  FILE *log_file;
public:
  FilePrint(const std::string file){init_log(file.c_str());};

  void init_log(const char *file);
  void init_log(const char *file, char* mode);

  int log(char *fmt, ...);
};

#endif