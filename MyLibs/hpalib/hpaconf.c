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

#include<stdio.h>
#include<string.h>

/* The macros VERSION, IPATH and LPATH are directly defined on the */
/* command line to the compiler.                                   */

#ifndef VERSION
#define VERSION "unknown"
#endif

#ifndef IPATH
#define IPATH ""
#endif

#ifndef LPATH
#define LPATH ""
#endif

/* The same is for the macros XMANTISSA_SIZE and XERR_... */

#ifndef XMANTISSA_SIZE
#define XMANTISSA_SIZE 7
#endif

#if !defined(XERR_IGN) && !defined(XERR_WARN) && !defined (XERR_EXIT)
#define XERR_DFL 1
#endif

#define XSIZE     2 * (XMANTISSA_SIZE + 1)
#define SGN_NBITS 1
#define EXP_NBITS 15
#define MNT_NBITS XMANTISSA_SIZE * 16
#define PREC_DIGS (XMANTISSA_SIZE * 4816) / 1000

#define DYN_RANGE     "     2^16384 > x > 2^(-16383)"
#define DYN_RANGE_10  "1.19*10^4932 > x > 1.68*10^-(4932)"

void
library_desc (void)
{
  fprintf (stdout,
	   "----- Features of the HPA library (including build options) -----\n\n");
  fprintf (stdout,
	   "Size of an extended precision floating point value (in bytes): %u\n",
	   XSIZE);
  fprintf (stdout, "Number of bits available for the sign:     %u\n",
	   SGN_NBITS);
  fprintf (stdout, "Number of bits available for the exponent: %u\n",
	   EXP_NBITS);
  fprintf (stdout, "Number of bits available for the mantissa: %u\n",
	   MNT_NBITS);
  fprintf (stdout, "Decimal digits of accuracy:               ~%u\n",
	   PREC_DIGS);
  fprintf (stdout,
	   "Dynamic range supported:     %s      i.e.\n \t\t\t     %s\n",
	   DYN_RANGE, DYN_RANGE_10);
  fprintf (stdout, "In case of floating point error\n");
#ifdef XERR_IGN
  fprintf (stdout, "nothing is signaled\n\n");
#elif defined(XERR_WARN)
  fprintf (stdout, "a warning message is printed on stderr\n\n");
#elif defined(XERR_EXIT)
  fprintf (stdout,
	   "the calling process is terminated through a call to exit(1)\n");
  fprintf (stdout, "after printing a suitable message on stderr\n\n");
#else
  fprintf (stdout,
	   "the global (extern) variable \'xErrNo\' is suitably set\n\n");
#endif
}

/* Possible options: -v, -c, -l, -n */

int
main (int argc, char *argv[])
{
  int i;
  unsigned char flag = 0;
  unsigned char v, c, l, n;

  if (argc == 1)
    {
      fprintf (stderr, "*** Usage:  %s [-v] [-n] or\n", argv[0]);
      fprintf (stderr, "***         %s [-c] [-l] [-n]\n\n", argv[0]);
      fprintf (stderr, "*** Meaning of the options:\n");
      fprintf (stderr,
	       "    -v      returns the current version of the HPA library,\n");
      fprintf (stderr,
	       "    -c      returns the flags to compile with HPA library,\n");
      fprintf (stderr,
	       "    -l      returns the flags to link against HPA library,\n");
      fprintf (stderr, "    -n      adds a newline at the end of the output.\n\n");
      library_desc ();
      return 0;
    }
  else if (argc > 5)
    {
      fprintf (stderr, "*** Usage:  %s [-v] [-n] or\n", argv[0]);
      fprintf (stderr, "***         %s [-c] [-l] [-n]\n\n", argv[0]);
      return 1;
    }
  else
    {
      for (i = 1; i < argc; i++)
	{
	  if (strcmp ("-v", argv[i]) == 0)
	    {
	      if (flag != 0 && flag != 8)
		{
		  fprintf (stderr, "*** Usage:  %s [-v] [-n] or\n", argv[0]);
		  fprintf (stderr, "***         %s [-c] [-l] [-n]\n\n", argv[0]);
		  return 1;
		}
	      else
		flag += 1;
	    }
	  else if (strcmp ("-c", argv[i]) == 0)
	    {
	      if ((flag % 2 == 1) || ((flag >> 1) % 2 == 1))
		{
		  fprintf (stderr, "*** Usage:  %s [-v] [-n] or\n", argv[0]);
		  fprintf (stderr, "***         %s [-c] [-l] [-n]\n\n", argv[0]);
		  return 1;
		}
	      else
		flag += 2;
	    }
	  else if (strcmp ("-l", argv[i]) == 0)
	    {
	      if ((flag % 2 == 1) || ((flag >> 2) % 2 == 1))
		{
		  fprintf (stderr, "*** Usage:  %s [-v] [-n] or\n", argv[0]);
		  fprintf (stderr, "***         %s [-c] [-l] [-n]\n\n", argv[0]);
		  return 1;
		}
	      else
		flag += 4;
	    }
	  else if (strcmp ("-n", argv[i]) == 0)
	    {
	      if ((flag >> 3) % 2 == 1)
		{
		  fprintf (stderr, "*** Usage:  %s [-v] [-n] or\n", argv[0]);
		  fprintf (stderr, "***         %s [-c] [-l] [-n]\n\n", argv[0]);
		  return 1;
		}
	      else
		flag += 8;
	    }
	  else
	    {
	      fprintf (stderr, "*** Usage:  %s [-v] [-n] or\n", argv[0]);
	      fprintf (stderr, "***         %s [-c] [-l] [-n]\n\n", argv[0]);
	      return 1;
	    }
	}			/* end for */
      v = flag % 2;
      c = (flag >> 1) % 2;
      l = (flag >> 2) % 2;
      n = (flag >> 3) % 2;
      if ((v))
	printf (VERSION);
      if ((c))
	{
	  printf (IPATH);
	  putchar (' ');
	}
      if ((l))
	{
	  printf (LPATH);
	  putchar (' ');
	  if ( strcmp (argv[0], "hpaxxconf") == 0 )
	    printf ("-lhpaxx -lhpa -lm");
	  else
	    printf ("-lhpa -lm");
	}
      if ((n))
	printf ("\n");
      else
	fflush (stdout);
      return 0;
    }				/* end else */
}
