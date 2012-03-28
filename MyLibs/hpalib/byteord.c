/*
   Copyright (C)  2000    Daniel A. Atkinson  <DanAtk@aol.com>
   Copyright (C)  2004    Ivano Primi  <ivprimi@libero.it>    

   This file is part of the HPA Library.

   The HPA Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The HPA Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the HPA Library; if not, write to the Free
   Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
   02110-1301 USA.
*/

/*
 *     Code to test the byte order used by your
 *     processor. 03-14-2005: Now it also writes the file hpaconf.h
*/

#include<stdio.h>
#include<assert.h>
#define CONFIG_H "./src/hpaconf.h"

/* The macros XMANTISSA_SIZE and XERR_* can be directly defined on the */
/* command line to the compiler.                                       */

#ifndef XMANTISSA_SIZE
#define XMANTISSA_SIZE 7
#endif

#if !defined(XERR_IGN) && !defined(XERR_WARN) && !defined (XERR_EXIT)
#define XERR_DFL 1
#endif

#ifndef VERSION
#define VERSION "unknown"
#endif

int
main (void)
{
  union
  {
    unsigned char cc[4];
    unsigned short s[2];
    unsigned long n;
  } u;
  int little_endian;
  FILE *pf;

  u.n = 0x12345678;
  little_endian = (u.s[0] == 0x5678 && u.s[1] == 0x1234 && u.cc[0] == 0x78 &&
		   u.cc[1] == 0x56 && u.cc[2] == 0x34 && u.cc[3] == 0x12);
  assert ((pf = fopen (CONFIG_H, "w")) != NULL);

  fprintf (pf, "#ifndef _HPACONF_H_\n#define _HPACONF_H_\n\n");
  fprintf (pf, "#define HPA_VERSION \t \"%s\"\n", VERSION);
  if ((little_endian))
    fprintf (pf, "#define XLITTLE_ENDIAN \t 1\n");
  else
    fprintf (pf, "#define XBIG_ENDIAN \t 1\n");
  fprintf (pf, "#define XDIM \t\t %u\n", XMANTISSA_SIZE);
  fprintf (pf, "#define XULONG_BITSIZE \t %u\n", (unsigned)(sizeof(unsigned long)*8));
#ifdef XERR_IGN
  fprintf (pf, "#define XERR_IGN \t 1\n\n");
#elif defined (XERR_WARN)
  fprintf (pf, "#define XERR_WARN \t 1\n\n");
#elif defined (XERR_EXIT)
  fprintf (pf, "#define XERR_EXIT \t 1\n\n");
#else
  fprintf (pf, "#define XERR_DFL \t 1\n\n");
#endif
  fprintf (pf, "#endif\n");
  assert (fclose (pf) == 0);
  return 0;
}
