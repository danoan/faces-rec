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
  bool _to_print;
public:
  FilePrint(const std::string file, bool to_print):_to_print(to_print){init_log(file.c_str());};

  void init_log(const char *file);
  void init_log(const char *file, char* mode);

  int log(char *fmt, ...);
};

#endif
