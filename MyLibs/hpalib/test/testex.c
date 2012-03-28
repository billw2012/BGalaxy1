/*
   testex - version 1.0
   Copyright (C)  2005    Ivano Primi  <ivprimi@libero.it>    

   This is the source file of the "testex" program.

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

#define BUFF_SIZE 1024

#define OK            0
#define EOF_REACHED   1
#define READING_ERROR 2
#define OUT_OF_MEMORY 3
/* *** */
#define OPEN_FAILED   4
#define WRONG_USAGE   5

char* read_line (FILE* pf, int* errcode)
{
  char buffer[BUFF_SIZE];
  char *ptr, *line = NULL;
  size_t lline = 0;

  while ((ptr = fgets (buffer, BUFF_SIZE,pf)))
    {
      lline += strlen (buffer);
      if (!line)
	ptr = (char*) calloc (lline + 1, sizeof(char));
      else
	ptr = (char*) realloc ((void*)line, (lline + 1) * sizeof(char));
      if (!ptr)
	{
	  if ((line))
	    free ((void*)line);
	  *errcode = OUT_OF_MEMORY;
	  return NULL;
	}
      else
	{
	  line = ptr;
	  strcat (line, buffer);
	}
      if (lline > 0 && line[lline-1] == '\n')
	break;
    }
  if (!ptr)
    *errcode = (ferror(pf)) ? READING_ERROR : EOF_REACHED; 
  else
    *errcode = OK;
  return line;
} 

int main(int argc, char** argv)
{
  FILE *fp;
  char *mark, *line;
  int errcode, printer_on = 0;
  
  if (argc == 1)
    {
      fp = stdin;
      mark = "";
    }
  else if (argc == 2)
    {
      fp = stdin;
      mark = argv[1];
    }
  else if (argc == 3)
    {
      if ( !(fp = fopen (argv[2], "r")) )
	{
	  fprintf (stderr, "\n*** Cannot open file \"%s\"\n", argv[2]);
	  return OPEN_FAILED;
	}
      mark = argv[1];
    }
  else
    {
      fprintf (stderr, "\n*** Usage:\n    testex [STRING] [FILE]\n");
      return WRONG_USAGE;
    }    
  do
    {
      line = read_line (fp, &errcode); 
      if ((line) && (strstr (line, mark)))
	printer_on = 1;
      if ((printer_on))
	{
	  if ((line))
	    {
	      fputs (line, stdout);
	      free ((void*) line);
	    }
	}
      else
	{
	  if ((line))
	    free ((void*) line);
	}
    } while (errcode == OK);
  switch (errcode)
    {
    case OUT_OF_MEMORY:
      fprintf (stderr, "\n*** Out of memory\n");
      return OUT_OF_MEMORY;
    case READING_ERROR:
      fprintf (stderr, "\n*** Input Error while reading from file \"%s\"\n", 
	       argv[2]);
      return READING_ERROR;
      /* otherwise EOF_REACHED */
    default:  
      return OK;
    }
}
