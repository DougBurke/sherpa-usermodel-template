C 
C  Copyright (C) 2008  Smithsonian Astrophysical Observatory
C
C
C  This program is free software; you can redistribute it and/or modify
C  it under the terms of the GNU General Public License as published by
C  the Free Software Foundation; either version 2 of the License, or
C  (at your option) any later version.
C
C  This program is distributed in the hope that it will be useful,
C  but WITHOUT ANY WARRANTY; without even the implied warranty of
C  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
C  GNU General Public License for more details.
C
C  You should have received a copy of the GNU General Public License along
C  with this program; if not, write to the Free Software Foundation, Inc.,
C  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
C
      integer function f_line(p, x, y, xsize)

C Sample Fortran user function, implements equation for a line.
C SMD 03/07/2008

      integer i, xsize
      real*8 p(*), x(xsize), y(xsize)
      f_line = 0

      do i=1,xsize,1
         y(i) = p(1) * x(i) + p(2)
      enddo

      return
      end
