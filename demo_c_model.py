# 
#  Copyright (C) 2008  Smithsonian Astrophysical Observatory
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
from sherpa.astro.ui import *

# Load data that we know a priori came from line equation
# y = x + 1 (i.e., m = 1, b = 1)

load_data(1, "data.dat")

# Load Fortran user and demonstrate fit

import userfuncs

load_user_model(userfuncs.c_line, "c_line")
add_user_pars("c_line", ["m","b"])

set_model(1, c_line)

c_line.m = 1.0
c_line.b = 1.0

plot_fit()

