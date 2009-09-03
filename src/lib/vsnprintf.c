/* Copyright (C) 2001 DJ Delorie, see http://www.delorie.com/bin/cvsweb.cgi/djgpp/copying.dj?rev=1.5 for details */
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <errno.h>

#define _IOWRT    000020
#define _IOSTRG   000400
#define _IONTERM  040000  /* file's handle not hooked by termios */

static __inline__ void __stropenw(FILE *p, char *str, int len)
{
  p->_flag = _IOWRT | _IOSTRG | _IONTERM;
  p->_ptr = str;
  p->_cnt = len;
}

static __inline__ void __strclosew(FILE *p)
{
  *p->_ptr = '\0';
}

int
vsnprintf(char *str, size_t n, const char *fmt, va_list ap)
{
  FILE _strbuf;
  int len;

  /* _cnt is an int in the FILE structure. To prevent wrap-around, we limit
   * n to between 0 and INT_MAX inclusively. */
  if (n > INT_MAX)
  {
    errno = EFBIG;
    return -1;
  }

  memset(&_strbuf, 0, sizeof(_strbuf));

  /* If n == 0, just querying how much space is needed. */
  if (n > 0)
    __stropenw(&_strbuf, str, n - 1);
  else
    __stropenw(&_strbuf, NULL, 0);

  len = _doprnt(fmt, ap, &_strbuf);

  /* Ensure nul termination */
  if (n > 0)
    __strclosew(&_strbuf);

  return len;
}
