/*
   body - version 1.0
   Copyright (C)  2005    Ivano Primi  <ivprimi@libero.it>    

   This is the source file of the "body" program.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public
   License along with this program; if not, write to the Free
   Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
   02110-1301 USA.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<errno.h>

int
line_completed (const char *s)
{
  if (!s)
    return 0;
  else
    {
      const char *t;

      for (t = s; *t != '\0' && *t != '\n'; t++);
      return (*t == '\n' ? 1 : 0);
    }
}

char *
str_dup (const char *s)
{
  if (!s)
    return NULL;
  else
    {
      size_t n = strlen (s);
      char *t;

      if ((t = (char *) malloc ((n + 1) * sizeof (char))))
	strcpy (t, s);
      return t;
    }
}

int
str_add (char **pstr, const char *s)
{
  if (!pstr)
    return -1;
  else if (*pstr == NULL)
    {
      if ((s))
	{
	  size_t n = strlen (s);

	  if ((*pstr = (char *) malloc ((n + 1) * sizeof (char))))
	    {
	      strcpy (*pstr, s);
	      return 0;
	    }
	  else
	    return -1;
	}
      else
	return 0;
    }
  else
    {
      if ((s))
	{
	  size_t m = strlen (*pstr);
	  size_t n = strlen (s);
	  char *t;

	  if ((t = (char *) realloc (*pstr, (m + n + 1) * sizeof (char))))
	    {
	      strcat (t, s);
	      *pstr = t;
	      return 0;
	    }
	  else
	    return -1;
	}
      else
	return 0;
    }
}

typedef struct
{
  FILE *dest;
  unsigned short size;
  char **line;
} queue;

queue
new_queue (unsigned short size, FILE * dest)
{
  queue q;

  q.dest = dest;
  if (size == 0)
    {
      q.size = 0;
      q.line = NULL;
    }
  else
    {
      if (!(q.line = (char **) calloc (size, sizeof (char *))))
	q.size = 0;
      else
	q.size = size;
    }
  return q;
}

int
put_string (queue * q, const char *str)
{
  if (!str)
    {
      /* Erase the contents of the queue */
      unsigned short k;

      for (k = 0; k < q->size; k++)
	{
	  if ((q->line[k]))
	    free ((void *) q->line[k]);
	}
      free ((void *) q->line);
      q->line = NULL;
      q->size = 0;
      return 0;
    }
  else
    {
      if (q->size > 0)
	{
	  if (!line_completed (q->line[0]))
	    return str_add (q->line, str);
	  else
	    {
	      long n;
	      int errcode = 0;

	      if ((q->line[q->size - 1]))
		{
		  errcode = fputs (q->line[q->size - 1], q->dest);
		  free ((void *) q->line[q->size - 1]);
		  q->line[q->size - 1] = NULL;
		}
	      for (n = q->size - 1; n > 0; q->line[n] = q->line[n - 1], n--);
	      if (!(q->line[0] = str_dup (str)))
		return -1;
	      else
		return ((errcode == EOF) ? -1 : 0);
	    }
	}
      else
	return (fputs (str, q->dest) == EOF ? -1 : 0);
    }
}

#define BUFF_SIZE 512

int
print_body (FILE * in, FILE * out,
	    unsigned short b_skip, unsigned short f_skip)
{
  char buffer[BUFF_SIZE];
  unsigned short k;

  for (k = 0; k < b_skip && (fgets (buffer, BUFF_SIZE, in));
       k += line_completed (buffer));
  if ((ferror (in)))
    return -1;
  else if (k == b_skip)
    {
      queue q;

      q = new_queue (f_skip, out);
      if (q.size == 0 && f_skip > 0)
	return -1;		/* Memory allocation failed */
      else
	{
	  int errcode = 0;

	  while ((fgets (buffer, BUFF_SIZE, in)) &&
		 !(errcode = put_string (&q, buffer)));
	  if ((ferror (in)) || (errcode))
	    return -1;
	  else
	    {
	      put_string (&q, NULL);
	      return 0;
	    }
	}
    }
  else
    return 0;
}

#define STR_2_OK    0
#define STR_2_UFLOW 1
#define STR_2_OFLOW 2
#define STR_2_NAN   3

int
str_2_short (const char *str, short *value)
{
  char *endp;
  char *t;
  long v;
  size_t l;

  errno = 0;
  l = strlen (str);
  if (!(t = (char *) calloc (l + 2, sizeof (char))))
    {
      fprintf (stderr, "*** Memory allocation failed\n");
      exit (EXIT_FAILURE);
    }
  else
    {
      strncpy (t, str, l + 1);
      strcat (t, ":");
    }
  v = strtol (t, &endp, 10);
  if ((strcmp (endp, ":")))
    {
      free ((void *) t);
      *value = 0;
      return STR_2_NAN;
    }
  else if (v < SHRT_MIN)
    {
      free ((void *) t);
      *value = SHRT_MIN;
      return STR_2_UFLOW;
    }
  else if (v > SHRT_MAX)
    {
      free ((void *) t);
      *value = SHRT_MAX;
      return STR_2_OFLOW;
    }
  else
    {
      free ((void *) t);
      *value = (short) v;
      return STR_2_OK;
    }
}

void
print_usage (void)
{
  fprintf (stderr, "Usage:   body -h\n");
  fprintf (stderr, "or       body [+m] [-n] [FILE]...\n\n");
  fprintf (stderr, "         where \'m\' and \'n\' must be integer\n");
  fprintf (stderr, "         values between 0 and %d\n\n", SHRT_MAX);
}

void
print_help (void)
{
  printf ("Usage:   body [+m] [-n] [FILE]...\n\n");
  printf ("         where \'m\' and \'n\' must be integer\n");
  printf ("         values between 0 and %d.\n", SHRT_MAX);
  printf ("         For each file in the given list of FILE(s), body\n");
  printf ("         prints on stdout all its lines but the first \'m\'\n");
  printf ("         and the last \'n\' ones.\n");
  printf ("         If \'FILE\' is an empty list, then body reads its\n");
  printf ("         lines from stdin.\n");
  printf ("         If one of the options \'+m\' or \'-n\' is omitted,\n");
  printf ("         then zero is used.\n");
  printf ("         If the first file in the list \'FILE\'\n");
  printf
    ("         starts by dash (\'-\') or by plus (\'+\'), then you have\n");
  printf ("         to put before its name the special option \'--\' to\n");
  printf ("         mark the end of the options list.\n\n");
}

int
main (int argc, char **argv)
{
  short b_skip, f_skip, m;
  int k, fnoop;

  b_skip = f_skip = 0;
  for (fnoop = 1; fnoop < argc; fnoop++)
    {
      if (strcmp (argv[fnoop - 1], "--") == 0
	  || (argv[fnoop][0] != '+' && argv[fnoop][0] != '-'))
	break;
    }
  /*
     fnoop is the index of the first non-option argument.
     argv[1], ..., argv[fnoop-1] are all the options
     passed on the command line.
   */
  switch (fnoop)
    {
    case 1:
      /*
         No option at all !
       */
      break;
    case 2:
      /*
         Only one option, stored in argv[1].
       */
      if (strcmp (argv[1], "--") != 0)
	{
	  if (strcmp (argv[1], "-h") == 0)
	    {
	      if (argc == 2)
		{
		  print_help ();
		  return 0;
		}
	      else
		{
		  print_usage ();
		  return 1;
		}
	    }
	  else if (str_2_short (argv[1], &m) == STR_2_OK)
	    {
	      if (argv[1][0] == '+')
		b_skip = m;
	      else
		f_skip = -m;
	    }
	  else
	    {
	      print_usage ();
	      return 1;
	    }
	}
      break;
    case 3:
      /*
         Two options, stored in argv[1] and argv[2]
         respectively. argv[2] might be "--", which 
         must be ignored. argv[1] cannot be "--".
         Neither argv[1] nor argv[2] may be "-h".
       */
      if (str_2_short (argv[1], &m) == STR_2_OK)
	{
	  if (argv[1][0] == '+')
	    b_skip = m;
	  else
	    f_skip = -m;
	}
      else
	{
	  print_usage ();
	  return 1;
	}
      if (strcmp (argv[2], "--") != 0)
	{
	  if (argv[2][0] != argv[1][0] &&
	      str_2_short (argv[2], &m) == STR_2_OK)
	    {
	      if (argv[2][0] == '+')
		b_skip = m;
	      else
		f_skip = -m;
	    }
	  else
	    {
	      print_usage ();
	      return 1;
	    }
	}
      break;
    case 4:
      /*
         Three options, stored in argv[1], argv[2]
         and argv[3] respectively. argv[3] must be "--".
         Neither argv[1] nor argv[2] can be "-h".
       */
      if (strcmp (argv[3], "--") != 0)
	{
	  print_usage ();
	  return 1;
	}
      else
	{
	  if (str_2_short (argv[1], &m) == STR_2_OK)
	    {
	      if (argv[1][0] == '+')
		b_skip = m;
	      else
		f_skip = -m;
	    }
	  else
	    {
	      print_usage ();
	      return 1;
	    }
	  if (argv[2][0] != argv[1][0] &&
	      str_2_short (argv[2], &m) == STR_2_OK)
	    {
	      if (argv[2][0] == '+')
		b_skip = m;
	      else
		f_skip = -m;
	    }
	  else
	    {
	      print_usage ();
	      return 1;
	    }
	}
      break;
    default:
      /*
         Too options !!
       */
      print_usage ();
      return 1;
    }
  /* end switch (fnoop) */
  if (fnoop == argc)
    {
      /*
         The list of the files is empty
       */
      if (print_body (stdin, stdout, b_skip, f_skip) == -1)
	{
	  fprintf (stderr,
		   "*** Error while reading or memory allocation failed\n");
	  return 1;
	}
      else
	return 0;
    }
  else
    {
      for (k = fnoop; k < argc; k++)
	{
	  FILE *in;

	  if (!(in = fopen (argv[k], "r")))
	    {
	      fprintf (stderr, "*** Cannot open %s\n", argv[k]);
	      return 1;
	    }
	  else
	    {
	      if (fnoop < argc - 1)
		fprintf (stdout, "==> %s <==\n", argv[k]);
	    }
	  if (print_body (in, stdout, b_skip, f_skip) == -1)
	    {
	      fclose (in);
	      fprintf (stderr,
		       "*** Error while reading or memory allocation failed\n");
	      return 1;
	    }
	  else
	    fclose (in);
	}
      return 0;
    }
}
