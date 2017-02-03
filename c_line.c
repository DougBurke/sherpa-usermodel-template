/* 
 *  Copyright (C) 2008  Smithsonian Astrophysical Observatory
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include <stdlib.h>
#include <math.h>
#include <float.h>

int c_line(const double* p, const double* x, const int xsize,
	   double* y, const int ysize) {

  int i;
  for (i=0; i < xsize; i++) {
    y[i] = p[0] * x[i] + p[1];
  }

  return EXIT_SUCCESS;
}

int c_line_int(const double* p, const double* xlo, const int xlosize,
	       const double* xhi, const int xhisize,
	       double* y, const int ysize) {

  /* If not implemented, leave this function empty and return success */
  return EXIT_SUCCESS;

}
