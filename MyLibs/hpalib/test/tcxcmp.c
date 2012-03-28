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
    Test:  cxis0  cxnot0  cxeq  cxneq  cxgt  cxge  cxlt  cxle

    Uses:  All the tested functions + strtocx  cxpr_print

    Input file:  data/cxcmp.dat
*/
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>		/* for isspace() */
#include"cxpre.h"

#define DECD 32

int
get_line (char *buffer, int size, FILE * fp)
{
  if ((fgets (buffer, size, fp)))
    {
      int i;

      for (i = 0; buffer[i] != '\0'; i++);
      if (i > 0 && buffer[i - 1] == '\n')
	buffer[--i] = '\0';
      return i;
    }
  else
    return -1;
}

typedef int (*cmpop) (struct cxpr, struct cxpr);
extern int xErrNo;

int
main (int na, char **av)
{
  struct cxpr z1, z2;
  FILE *fp;
  char num[1024], *endptr;
  const char *nlist[] = {
    "==", "!=", "> ", ">=", "< ", "<=",
    NULL
  };
  cmpop oplist[] = {
    cxeq, cxneq, cxgt, cxge, cxlt, cxle
  };
  int i;

  if (na != 2)
    {
      printf ("para: input_file\n");
      exit (-1);
    }
  fp = fopen (*++av, "r");
  printf ("TEST OF THE COMPARISON'S OPERATORS FOR COMPLEX VALUES\n\n");
  while (get_line (num, 1024, fp) != -1)
    {
      printf ("*string in : \t%s\n", num);
      z1 = strtocx (num, &endptr);
      for (; isspace (*endptr) != 0; endptr++);
      z2 = strtocx (endptr, NULL);
      printf ("z1 = ");
      cxpr_print (stdout, z1, 0, 1, DECD);
      printf ("\nz2 = ");
      cxpr_print (stdout, z2, 0, 1, DECD);
      putchar ('\n');
      if ((cxis0 (&z1)))
	printf ("z1 == 0 \t");
      if ((cxis0 (&z2)))
	printf ("z2 == 0 \t");
      if ((cxnot0 (&z1)))
	printf ("z1 != 0 \t");
      if ((cxnot0 (&z2)))
	printf ("z2 != 0 \t");
      for (i = 0; nlist[i] != NULL; i++)
	{
	  if ((*oplist[i]) (z1, z2))
	    printf ("z1 %s z2\t", nlist[i]);
	}
      putchar ('\n');
    }
  fclose (fp);
  return 0;
}

/*  Test output

TEST OF THE COMPARISON'S OPERATORS FOR COMPLEX VALUES

*string in : 	0 -0i	 0.0000000000000000 +0.000000000000i
z1 = +0.00000000000000000000000000000000+0.00000000000000000000000000000000i
z2 = +0.00000000000000000000000000000000+0.00000000000000000000000000000000i
z1 == 0 	z2 == 0 	z1 == z2	z1 >= z2	z1 <= z2	
*string in : 	0 -1i	 0 +1i
z1 = +0.00000000000000000000000000000000-1.00000000000000000000000000000000i
z2 = +0.00000000000000000000000000000000+1.00000000000000000000000000000000i
z1 != 0 	z2 != 0 	z1 != z2	z1 <  z2	z1 <= z2	
*string in : 	-1	 1
z1 = -1.00000000000000000000000000000000+0.00000000000000000000000000000000i
z2 = +1.00000000000000000000000000000000+0.00000000000000000000000000000000i
z1 != 0 	z2 != 0 	z1 != z2	z1 <  z2	z1 <= z2	
*string in : 	0 +0i    0.000000000000051 +0i
z1 = +0.00000000000000000000000000000000+0.00000000000000000000000000000000i
z2 = +0.0000000000000509999999999999999999999999999991+0.00000000000000000000000000000000i
z1 == 0 	z2 != 0 	z1 != z2	z1 <  z2	z1 <= z2	
*string in : 	0 +0i	 0 +0.0000000000000031i
z1 = +0.00000000000000000000000000000000+0.00000000000000000000000000000000i
z2 = +0.00000000000000000000000000000000+0.00000000000000309999999999999999999999999999994i
z1 == 0 	z2 != 0 	z1 != z2	z1 <  z2	z1 <= z2	
*string in : 	0 +0i	-0.000000000000051 +0i
z1 = +0.00000000000000000000000000000000+0.00000000000000000000000000000000i
z2 = -0.0000000000000509999999999999999999999999999991+0.00000000000000000000000000000000i
z1 == 0 	z2 != 0 	z1 != z2	z1 >  z2	z1 >= z2	
*string in : 	0 +0i 	 0 -0.0000000000000031i
z1 = +0.00000000000000000000000000000000+0.00000000000000000000000000000000i
z2 = +0.00000000000000000000000000000000-0.00000000000000309999999999999999999999999999994i
z1 == 0 	z2 != 0 	z1 != z2	z1 >  z2	z1 >= z2	
*string in : 	0 +0i	 0.00000000000000423 -0.00000000000000065i
z1 = +0.00000000000000000000000000000000+0.00000000000000000000000000000000i
z2 = +0.00000000000000422999999999999999999999999999992-0.000000000000000649999999999999999999999999999987i
z1 == 0 	z2 != 0 	z1 != z2	
*string in : 	0 +0i	-0.00000000000721 +0.000000000000008231i
z1 = +0.00000000000000000000000000000000+0.00000000000000000000000000000000i
z2 = -0.00000000000720999999999999999999999999999988+0.00000000000000823099999999999999999999999999983i
z1 == 0 	z2 != 0 	z1 != z2	
*string in : 	34.67 +43.21i 	 34.670000000000051 +52.22i
z1 = +34.6699999999999999999999999999999+43.2099999999999999999999999999999i
z2 = +34.6700000000000509999999999999999+52.2199999999999999999999999999999i
z1 != 0 	z2 != 0 	z1 != z2	z1 <  z2	z1 <= z2	
*string in : 	34.67 +43.21i	 34.67 +43.2100000000098i
z1 = +34.6699999999999999999999999999999+43.2099999999999999999999999999999i
z2 = +34.6699999999999999999999999999999+43.2100000000097999999999999999999i
z1 != 0 	z2 != 0 	z1 != z2	z1 <  z2	z1 <= z2	
*string in : 	-45.55 -56.78i	-45.550000000000000012 -56.78i
z1 = -45.5500000000000000000000000000000-56.7799999999999999999999999999999i
z2 = -45.5500000000000000119999999999999-56.7799999999999999999999999999999i
z1 != 0 	z2 != 0 	z1 != z2	z1 >  z2	z1 >= z2	
*string in : 	-45.55 -56.78i	-45.55 -56.7800000000098i
z1 = -45.5500000000000000000000000000000-56.7799999999999999999999999999999i
z2 = -45.5500000000000000000000000000000-56.7800000000097999999999999999999i
z1 != 0 	z2 != 0 	z1 != z2	z1 >  z2	z1 >= z2	
*string in : 	2.34 -5.78i	 2.35 -5.65i
z1 = +2.34000000000000000000000000000000-5.78000000000000000000000000000000i
z2 = +2.35000000000000000000000000000000-5.65000000000000000000000000000000i
z1 != 0 	z2 != 0 	z1 != z2	z1 <  z2	z1 <= z2	
*string in : 	2.34 -5.78i	 1.22 -5.7800000002i
z1 = +2.34000000000000000000000000000000-5.78000000000000000000000000000000i
z2 = +1.22000000000000000000000000000000-5.78000000019999999999999999999999i
z1 != 0 	z2 != 0 	z1 != z2	z1 >  z2	z1 >= z2	
*string in : 	789.345 +785i	 32.00012 +786.21i
z1 = +789.344999999999999999999999999997+784.999999999999999999999999999998i
z2 = +32.0001200000000000000000000000000+786.209999999999999999999999999997i
z1 != 0 	z2 != 0 	z1 != z2	
*string in : 	-65.98700 +87.654i  	0.9978 +29.331i
z1 = -65.9869999999999999999999999999998+87.6539999999999999999999999999998i
z2 = +0.997799999999999999999999999999996+29.3310000000000000000000000000000i
z1 != 0 	z2 != 0 	z1 != z2	
*string in : 	56.2765e3 -4321.23e-4i	56.2765e3 -4321.23e-4i
z1 = +56276.4999999999999999999999999997-0.432122999999999999999999999999998i
z2 = +56276.4999999999999999999999999997-0.432122999999999999999999999999998i
z1 != 0 	z2 != 0 	z1 == z2	z1 >= z2	z1 <= z2	
*/
