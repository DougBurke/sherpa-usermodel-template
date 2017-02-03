// 
//  Copyright (C) 2008, 2016  Smithsonian Astrophysical Observatory
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

#ifndef __sherpa_usermodel_glue_hh__
#define __sherpa_usermodel_glue_hh__

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION

#include <Python.h>
#include <numpy/arrayobject.h>
#include "array.hh"

#include <cmath>
#include <cfloat>

// The following is taken from Sherpa's sherpa/include/sherpa/extension.hh
//
typedef sherpa::Array< double, NPY_DOUBLE > DoubleArray;

typedef int (*converter)( PyObject*, void* );

#define CONVERTME(arg) ((converter) sherpa::convert_to_contig_array<arg>)

#if PY_MAJOR_VERSION >= 3
#define PY3
#endif

#ifdef PY3

#define SHERPAMOD(name, fctlist) \
static struct PyModuleDef module##name = {\
PyModuleDef_HEAD_INIT, \
#name, \
NULL, \
-1, \
fctlist \
}; \
\
PyMODINIT_FUNC PyInit_##name(void) { \
  import_array(); \
  return PyModule_Create(&module##name); \
}

#else

#define SHERPAMOD(name, fctlist) \
PyMODINIT_FUNC init##name(void);\
PyMODINIT_FUNC \
init##name(void) \
{ \
  import_array(); \
  Py_InitModule( (char*)#name, fctlist ); \
}

#endif

// end of extension.hh


namespace usermodel {

  // This is the fct1d template specialized to double arrays,
  // since this is the simplest way to get it to work with a
  // recent gcc.
  //
template <npy_intp NumPars,
	  int (*PtFunc)( const double* p, const double* x, 
                         const int xsize, double* out, 
                         const int outsize ),
	  int (*IntFunc)( const double* p, const double* xlo, 
                          const int xlosize, const double* xhi,
                          const int xhisize, double* out, 
                          const int outsize )>
PyObject* fct1d_dbl( PyObject* self, PyObject* args )
{
  
  DoubleArray p;
  DoubleArray xlo;
  DoubleArray xhi;

  if( !PyArg_ParseTuple( args, (char*)"O&O&|O&",
			 CONVERTME( DoubleArray ), &p,
			 CONVERTME( DoubleArray ), &xlo,
			 CONVERTME( DoubleArray ), &xhi ) )
    return NULL;  
    
  npy_intp npars = p.get_size();
  
  if ( NumPars != npars ) {
    PyErr_Format( PyExc_TypeError, (char*)"expected %ld parameters, got %ld",
		  long( NumPars ), long( npars ) );
    return NULL;
  }
  
  npy_intp nelem = xlo.get_size();
  
  if ( xhi && ( xhi.get_size() != nelem ) ) {
      PyErr_SetString( PyExc_TypeError,
                       (char*)"input array sizes do not match" );
      return NULL;
  }
  
  DoubleArray result;
  if ( EXIT_SUCCESS != result.create( xlo.get_ndim(), xlo.get_dims() ) )
    return NULL;
  
  const double* pars = ( const_cast< double* >( &p[0] ) );
  const double* xlo_arr = ( const_cast< double* >( &xlo[0] ) );
  const double* xhi_arr = NULL;
  if (xhi) {
    xhi_arr = ( const_cast< double* >( &xhi[0] ) );
  }
  double* result_arr = ( const_cast< double* >( &result[0] ) );
  
  if ( !xhi ) {
    if (PtFunc( pars, xlo_arr, nelem, result_arr, nelem ) != EXIT_SUCCESS)
      return NULL;
  }
  else {
    if (IntFunc( pars, xlo_arr, nelem, xhi_arr, nelem, 
		 result_arr, nelem ) != EXIT_SUCCESS)
      return NULL;
  }

  return result.return_new_ref();
  
}

  // This is the original template used by the usermodel code.
  // It is currently unused.
  //
template <typename ArrayType,
	  typename DataType,
	  npy_intp NumPars,
	  npy_intp (*PtFunc)( const DataType* p, const DataType* x, 
			      const npy_intp xsize, DataType* out, 
			      const npy_intp outsize ),
	  npy_intp (*IntFunc)( const DataType* p, const DataType* xlo, 
			       const npy_intp xlosize, const DataType* xhi,
			       const npy_intp xhisize, DataType* out, 
			       const npy_intp outsize )>
PyObject* fct1d( PyObject* self, PyObject* args )
{
  
  ArrayType p;
  ArrayType xlo;
  ArrayType xhi;

  if( !PyArg_ParseTuple( args, (char*)"O&O&|O&",
			 CONVERTME( ArrayType ), &p,
			 CONVERTME( ArrayType ), &xlo,
			 CONVERTME( ArrayType ), &xhi ) )
    return NULL;  
    
  npy_intp npars = p.get_size();
  
  if ( NumPars != npars ) {
    PyErr_Format( PyExc_TypeError, (char*)"expected %ld parameters, got %ld",
		  long( NumPars ), long( npars ) );
    return NULL;
  }
  
  npy_intp nelem = xlo.get_size();
  
  if ( xhi && ( xhi.get_size() != nelem ) ) {
      PyErr_SetString( PyExc_TypeError,
                       (char*)"input array sizes do not match" );
      return NULL;
  }
  
  ArrayType result;
  if ( EXIT_SUCCESS != result.create( xlo.get_ndim(), xlo.get_dims() ) )
    return NULL;
  
  const DataType* pars = ( const_cast< DataType* >( &p[0] ) );
  const DataType* xlo_arr = ( const_cast< DataType* >( &xlo[0] ) );
  const DataType* xhi_arr = NULL;
  if (xhi) {
    xhi_arr = ( const_cast< DataType* >( &xhi[0] ) );
  }
  DataType* result_arr = ( const_cast< DataType* >( &result[0] ) );
  
  if ( !xhi ) {
    if (PtFunc( pars, xlo_arr, nelem, result_arr, nelem ) != EXIT_SUCCESS)
      return NULL;
  }
  else {
    if (IntFunc( pars, xlo_arr, nelem, xhi_arr, nelem, 
		 result_arr, nelem ) != EXIT_SUCCESS)
      return NULL;
  }

  return result.return_new_ref();
  
}

  // To use the 2D template probably requires a specialization
  // to the double type, as done above with fct1d_dbl / fct1d.
  //
template <typename ArrayType,
	  typename DataType,
	  npy_intp NumPars,
          npy_intp (*PtFunc)( const DataType* p, const DataType* x0, 
			      const npy_intp x0size, const DataType* x1,
			      const npy_intp x1size, DataType* out, 
			      const npy_intp outsize ),
	  npy_intp (*IntFunc)( const DataType* p, const DataType* x0lo, 
			       const npy_intp x0losize, const DataType* x0hi, 
			       const npy_intp x0hisize, const DataType* x1lo,
			       const npy_intp x1losize, const DataType* x1hi,
			       const npy_intp x1hisize, DataType* out, 
			       const npy_intp outsize )>
PyObject* fct2d( PyObject* self, PyObject* args )
{
  
  ArrayType p;
  ArrayType x0lo;
  ArrayType x1lo;
  ArrayType x0hi;
  ArrayType x1hi;
  
  if ( !PyArg_ParseTuple( args, (char*)"O&O&O&|O&O&",
			  CONVERTME( ArrayType ), &p,
			  CONVERTME( ArrayType ), &x0lo,
			  CONVERTME( ArrayType ), &x1lo,
			  CONVERTME( ArrayType ), &x0hi,
			  CONVERTME( ArrayType ), &x1hi ) )
    return NULL;
  
  npy_intp npars = p.get_size();
  
  if ( NumPars != npars ) {
    PyErr_Format( PyExc_TypeError, (char*)"expected %ld parameters, got %ld",
		  long( NumPars ), long( npars ) );
    return NULL;
  }
  
  if ( x0hi && !x1hi )  {
    PyErr_SetString( PyExc_TypeError,
		     (char*)"expected 3 or 5 arguments, got 4" );
    return NULL;
  }
  
  npy_intp nelem = x0lo.get_size();
  
  if ( ( x1lo.get_size() != nelem ) ||
       ( x0hi &&
	 ( ( x0hi.get_size() != nelem ) ||
	   ( x1hi.get_size() != nelem ) ) ) ) {
    PyErr_SetString( PyExc_TypeError,
		     (char*)"input array sizes do not match" );
    return NULL;
  }
  
  ArrayType result;
  if ( EXIT_SUCCESS != result.create( x0lo.get_ndim(), x0lo.get_dims() ) )
    return NULL;
  
  const DataType* pars = ( const_cast< DataType* >( &p[0] ) );
  const DataType* x0lo_arr = ( const_cast< DataType* >( &x0lo[0] ) );
  const DataType* x1lo_arr = ( const_cast< DataType* >( &x1lo[0] ) );
  const DataType* x0hi_arr = NULL;
  const DataType* x1hi_arr = NULL;
  if (x0hi) {
    x0hi_arr = ( const_cast< DataType* >( &x0hi[0] ) );
    x1hi_arr = ( const_cast< DataType* >( &x1hi[0] ) );
  }
  DataType* result_arr = ( const_cast< DataType* >( &result[0] ) );

  if ( !x0hi ) {
    if (PtFunc( pars, x0lo_arr, nelem, x1lo_arr, nelem, 
		result_arr, nelem ) != EXIT_SUCCESS)
      return NULL;
  }
  else {
    if (IntFunc( pars, x0lo_arr, nelem, x0hi_arr, nelem,
		 x1lo_arr, nelem, x1hi_arr, nelem, 
		 result_arr, nelem ) != EXIT_SUCCESS)
      return NULL;
  }

  return result.return_new_ref();
  
}

} // namespace usermodel

#define FCTSPEC(name, func) \
  { (char*)#name, (PyCFunction)func, METH_VARARGS, NULL }

/***
#define _MODELFCTPTR(name) \
  name< double, DoubleArray >

#define _USERMODELFCTSPEC(ptfunc, ftype, npars)       \
  FCTSPEC(ptfunc, (ftype< DoubleArray, double, npars,        \
                   _MODELFCTPTR(ptfunc),             \
                   _MODELFCTPTR(ptfunc##_int)         \
                   >))
***/

#define _MODELFCTPTR(name) name

#define _USERMODELFCTSPEC(ptfunc, ftype, npars)       \
  FCTSPEC(ptfunc, (ftype< npars,        \
                   _MODELFCTPTR(ptfunc),             \
                   _MODELFCTPTR(ptfunc##_int)         \
                   >))

#define USERMODELFCT1D(ptfunc, npars) \
  _USERMODELFCTSPEC(ptfunc, usermodel::fct1d_dbl, npars)

/***

To use this you will probably need to create a fct2d_dbl version.

#define USERMODELFCT2D(ptfunc, npars) \
  _USERMODELFCTSPEC(ptfunc, usermodel::fct2d, npars)
***/

#endif /* __sherpa_usermodel_glue_hh__ */
