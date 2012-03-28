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
    Test:  inttox  uinttox

    Uses:  inttox  uinttox  xpr_print
    
    Input file:  data/inttox32.dat
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "xpre.h"

#define BUFF_SIZE 1024		/* 1 Kb */
int decd = 12;

int 
main (int na, char **av)
{
  int i;
  struct xpr x;
  char buffer[BUFF_SIZE];
  FILE* fp;
  
  if (na != 2)
    {
      printf ("para: input_file\n");
      exit (-1);
    }
  fp = fopen (*++av, "r");
  printf ("     Test of the functions inttox() and uinttox()\n");
  while (fgets (buffer, BUFF_SIZE, fp) != NULL)
    {
      for (i = 0; buffer[i] != '\0'; i++);
      if (i > 0 && buffer[i - 1] == '\n')
	buffer[i - 1] = '\0';
      if (*buffer == 'U')
	{
	  unsigned long u; 

	  u = strtoul (buffer + 1, NULL, 10);
	  x = uinttox (u);
	  printf ("<< %lu\n>> ", u);
	  xpr_print (stdout, x, 0, 0, decd); 
	  putchar ('\n');
	}
      else
	{
	  long n;

	  n = strtol (buffer, NULL, 10);
	  x = inttox (n);
	  printf ("<< %ld\n>> ", n);
	  xpr_print (stdout, x, 0, 0, decd); 
	  putchar ('\n');
	}
    }
  return 0;
}

/* Test output

     Test of the functions inttox() and uinttox()
<< -2147483648
>> -2147483648.000
<< -2147483648
>> -2147483648.000
<< -2147483648
>> -2147483648.000
<< -2147483647
>> -2147483647.000
<< -2147483646
>> -2147483646.000
<< -2100005403
>> -2100005403.000
<< -1200980077
>> -1200980077.000
<< -1000000000
>> -1000000000.000
<< -100000000
>> -100000000.0000
<< -49882197
>> -49882197.00000
<< -10000000
>> -10000000.00000
<< -1000000
>> -1000000.000000
<< -765428
>> -765428.0000000
<< -230192
>> -230192.0000000
<< -10000
>> -10000.00000000
<< -5423
>> -5423.000000000
<< -1000
>> -1000.000000000
<< -897
>> -897.0000000000
<< -438
>> -438.0000000000
<< -208
>> -208.0000000000
<< -100
>> -100.0000000000
<< -65
>> -65.00000000000
<< -10
>> -10.00000000000
<< -7
>> -7.000000000000
<< -4
>> -4.000000000000
<< -2
>> -2.000000000000
<< -1
>> -1.000000000000
<< 0
>> 0.000000000000
<< 1
>> 1.000000000000
<< 2
>> 2.000000000000
<< 4
>> 4.000000000000
<< 9
>> 9.000000000000
<< 12
>> 12.00000000000
<< 77
>> 77.00000000000
<< 100
>> 100.0000000000
<< 362
>> 362.0000000000
<< 589
>> 589.0000000000
<< 876
>> 876.0000000000
<< 1000
>> 1000.000000000
<< 6633
>> 6633.000000000
<< 10000
>> 10000.00000000
<< 278996
>> 278996.0000000
<< 1300000
>> 1300000.000000
<< 10000000
>> 10000000.00000
<< 28719886
>> 28719886.00000
<< 72192225
>> 72192225.00000
<< 100000000
>> 100000000.0000
<< 600000000
>> 600000000.0000
<< 1000000000
>> 1000000000.000
<< 1500891500
>> 1500891500.000
<< 1710000980
>> 1710000980.000
<< 1988210923
>> 1988210923.000
<< 2147483646
>> 2147483646.000
<< 2147483647
>> 2147483647.000
<< 2147483647
>> 2147483647.000
<< 2147483647
>> 2147483647.000
<< 2147483647
>> 2147483647.000
<< 0
>> 0.000000000000
<< 1
>> 1.000000000000
<< 2
>> 2.000000000000
<< 6
>> 6.000000000000
<< 17
>> 17.00000000000
<< 83
>> 83.00000000000
<< 186
>> 186.0000000000
<< 432
>> 432.0000000000
<< 665
>> 665.0000000000
<< 934
>> 934.0000000000
<< 1000
>> 1000.000000000
<< 2340
>> 2340.000000000
<< 5398
>> 5398.000000000
<< 10000
>> 10000.00000000
<< 64008
>> 64008.00000000
<< 87198
>> 87198.00000000
<< 100000
>> 100000.0000000
<< 706132
>> 706132.0000000
<< 1000000
>> 1000000.000000
<< 3211984
>> 3211984.000000
<< 8712092
>> 8712092.000000
<< 10000000
>> 10000000.00000
<< 310000870
>> 310000870.0000
<< 871290124
>> 871290124.0000
<< 1000000000
>> 1000000000.000
<< 1289000182
>> 1289000182.000
<< 2147483647
>> 2147483647.000
<< 2147483648
>> 2147483648.000
<< 2290128710
>> 2290128710.000
<< 2788000000
>> 2788000000.000
<< 3000000000
>> 3000000000.000
<< 3600000000
>> 3600000000.000
<< 4280120000
>> 4280120000.000
<< 4294967295
>> 4294967295.000
<< 4294967295
>> 4294967295.000
<< 4294967295
>> 4294967295.000
<< 4294967295
>> 4294967295.000
<< 4294967295
>> 4294967295.000
*/
