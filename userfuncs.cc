// 
//  Copyright (C) 2008  Smithsonian Astrophysical Observatory
//
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
#include "glue.hh"

// User function prototypes
extern "C" {

  // USER CONTENT HERE
  // Function prototypes for c_line example
  int c_line(const double* p, const double* x, const int xsize,
	     double* y, const int ysize);
  int c_line_int(const double* p, const double* xlo, const int xlosize,
		 const double* xhi, const int xhisize,
		 double* y, const int ysize);
  // END USER CONTENT HERE
}

static PyMethodDef UserFuncs[] = {

  // Here we add function prototypes to the list of functions
  // Python will know about when this module is imported.
  //
  // Functions are added via calls to
  //
  // USERMODELFCT1D( func_proto, num_parameters ),
  //
  // where func_proto is the reference to the user-written function,
  // and num_parameters is the number of parameters of the user written
  // function.

  // USER CONTENT HERE

  // c_line is the line function from the included example.  The
  // number of parameter is 2 (implements y = mx + b, and [m,b] is
  // the array of parameters).  The arrays of parameters is always
  // passed as the first argument to the user function.

  USERMODELFCT1D( c_line, 2 ),
  
  // END USER CONTENT HERE
  
  { NULL, NULL, 0, NULL }
  
};


PyMODINIT_FUNC
inituserfuncs(void)
{ 
  import_array();
  
  Py_InitModule( (char*)"userfuncs", UserFuncs );
}
