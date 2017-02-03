#
#  Copyright (C) 2008, 2016  Smithsonian Astrophysical Observatory
#
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License along
#  with this program; if not, write to the Free Software Foundation, Inc.,
#  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#
from numpy.distutils.core import setup, Extension
import platform
import sys

c_libs = []
c_lib_dirs = []

# Pick names of file(s) off of command line

c_files = ['userfuncs.cc']

while ((len(sys.argv) > 1) and (sys.argv[1].count('=') == 1) and
       (not sys.argv[1].startswith('--'))):
    var, val = sys.argv.pop(1).split('=')
    # Option 'cfiles' lists comma-separated set of C/C++ files
    if (var == 'cfiles'):
        c_files = c_files + val.split(',')
    if (var == 'libs'):
        c_libs = c_libs + val.split(',')
    if (var == 'libdirs'):
        c_lib_dirs = c_lib_dirs + val.split(',')

setup(name='userfuncs',
      version='1.1.0',
      author='Chandra X-ray Center',
      # author_email='@cfa.harvard.edu',
      ext_modules=[Extension('userfuncs',
                             c_files,
                             library_dirs=c_lib_dirs,
                             libraries=c_libs,
                             depends=['array.hh', 'glue.hh'])]
      )
