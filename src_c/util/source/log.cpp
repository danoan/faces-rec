#include "../headers/log.h"

#define STRING_LENGTH 240                    /* something 'safe'             */

void FilePrint::init_log(const char *file){
  return init_log(file,"w");
}

void FilePrint::init_log(const char *file, char* mode){
  char filepath[STRING_LENGTH];
  sprintf(filepath,"%s/%s",Config::LOG_PATH.c_str(),file);

  printf("%s\n",filepath);

  char error[STRING_LENGTH];
  if (!(log_file = fopen(filepath, mode))) {     /* open file for mode           */
    sprintf(error, "init_log() failed to open %s.\n", filepath);
    perror(error);
    exit(666);                               /* message and exit on fail     */
  }
}

int FilePrint::log(char *fmt, ...){
  va_list ap;                                /* special type for variable    */
  char format[STRING_LENGTH];                /* argument lists               */
  int count = 0;
  int i, j;                                  /* Need all these to store      */
  char c;                                    /* values below in switch       */
  double d;
  unsigned u;
  char *s;
  void *v;
  
  if(!_to_print) return 0;

  va_start(ap, fmt);                         /* must be called before work   */
  while (*fmt) {
    for (j = 0; fmt[j] && fmt[j] != '%'; j++)
      format[j] = fmt[j];                    /* not a format string          */
    if (j) {
      format[j] = '\0';
      count += fprintf(log_file, format);    /* log it verbatim              */
      fmt += j;
    } else {
      for (j = 0; !isalpha(fmt[j]); j++) {   /* find end of format specifier */
        format[j] = fmt[j];
        if (j && fmt[j] == '%')              /* special case printing '%'    */
          break;
      }
      format[j] = fmt[j];                    /* finish writing specifier     */
      if(format[j]=='l'){
          format[j+1] = fmt[j+1];
          format[j + 2] = '\0';
          fmt += j + 2;
      }else{
          format[j + 1] = '\0';                  /* don't forget NULL terminator */  
          fmt += j + 1;
      }      
      
      
      switch (format[j]) {                   /* cases for all specifiers     */
      case 'd':
      case 'i':                              /* many use identical actions   */
        i = va_arg(ap, int);                 /* process the argument         */
        count += fprintf(log_file, format, i); /* and log it                 */
        break;
      case 'o':
      case 'x':
      case 'X':
      case 'u':
        u = va_arg(ap, unsigned);
        count += fprintf(log_file, format, u);
        break;
      case 'c':
        c = (char) va_arg(ap, int);          /* must cast!                   */
        count += fprintf(log_file, format, c);
        break;
      case 's':
        s = va_arg(ap, char *);
        count += fprintf(log_file, format, s);
        break;
      case 'f':
      case 'e':
      case 'E':
      case 'g':
      case 'G':
        d = va_arg(ap, double);
        count += fprintf(log_file, "%.8f", d);
        break;
      case 'p':
        v = va_arg(ap, void *);
        count += fprintf(log_file, format, v);
        break;
      case 'n':
        count += fprintf(log_file, "%d", count);
        break;
      case 'l':
        if(format[j+1]=='f'){
          d = va_arg(ap, double);
          count += fprintf(log_file, format, d);
        }else if(format[j+1]=='d'){
          i = va_arg(ap, int);
          count += fprintf(log_file, format, i);
        }else if(format[j+1]=='u'){
          u = va_arg(ap, unsigned);
          count += fprintf(log_file, format, u);
        }
        break;
      case '%':
        count += fprintf(log_file, "%%");
        break;
      default:
        fprintf(stderr, "Invalid format specifier in log().\n");
      }
    }
  }
  fflush(log_file);
  va_end(ap);                                /* clean up                     */

  return count;
}
