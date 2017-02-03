This file contains instructions for building and running the example
code that adds a C function to a Sherpa user model.

The relevant files are:
	
demo_c_model.py		Example session reading simple data file,
			defining user model, and overplotting data
			and model.

c_line.c		C function implementing equation for a line, 
			y = mx + b.

setup.py		Python distutils file for creating C extension
                        module (user needs to add name of C file
                        here).

userfuncs.cc		File listing function prototypes for all user
                        functions (user needs to add user function
                        prototypes here).

There are also two interface files--array.hh and glue.hh--that need to
be present, but that the user need NEVER touch or look at.  (These
contain copies of Python/C interface functions that Sherpa uses to
pass arrays from Python to Sherpa C functions.  The user should never
need to modify this code.)

Instructions:

1)  Ensure C and C++ compilers can be found.

    The first step is to ensure that the right versions of the C and
    C++ compilers are found.  If you built CIAO from source, you will
    use the same compilers you used to build CIAO.

    If you installed a binary build of CIAO, you need to look up which
    version of the compilers were used for that binary build.  For
    example, the Linux FC4 build was built with GCC 3.4.5.  So, if you
    do

    % gcc -v
    % g++ -v 

    and the version is 3.4.5, that's fine.  But if 3.4.5 is not in
    your PATH, you either need to add that directory to your path, or
    set CC and CXX environment variables.

    For example, suppose GCC 3.4.5 is installed in /usr/local/bin, and
    that directory is not in your PATH.  Set the variables:

    % setenv CC /usr/local/bin/gcc
    % setenv CXX /usr/local/bin/g++

    When these environment variables are set, then the Python setup.py
    file included in this tar file will find these compilers.  Use of
    the setup.py file is discussed in section 3) below.

2)  Add C user function prototypes to userfuncs.cc file.

    This file lists all C functions to be added to the userfuncs
    module.  All the user has to do is to add their function
    prototypes where the comment

    // USER CONTENT HERE

    indicates.

    For example, here we include function prototypes for the functions
    defined in c_line.c.  The prototypes added here are:

      // USER CONTENT HERE
      // Function prototypes for c_line example
      int c_line(const double* p, const double* x, const int xsize,
                 double* y, const int ysize);
      int c_line_int(const double* p, const double* xlo, const int xlosize,
                     const double* xhi, const int xhisize,
                     double* y, const int ysize);
      // END USER CONTENT HERE

    The user can add their own function prototypes here.

    Some things to note:

    . The user function has to have arguments in the same order given
      here:  for c_line, parameters, x, xsize, y and ysize; for
      c_line_int, parameters, xlo, xlosize, xhi, xhisize, y and
      ysize.  The code will not compile if the user function has
      arguments in a different order.

    . For every function, there must be an unbinned and a binned form.
      This is seen in this example, with c_line and c_line_int.

      But, since in this example we are only interested in the
      unbinned form, the binned form, c_line_int, does nothing.
      Nevertheless, the function must be present, so that its function
      prototype can be listed in userfuncs.cc.   

    . Since it is plain C code, the function prototypes must be
      enclosed in a block:

      extern "C" {
        // prototypes
      }

      See userfuncs.cc for the complete example.  If the user is
      adding C++, the C++ functions prototypes should NOT be enclosed
      in such a block. 

    There is one more place where the function prototype must be
    added.  Later in the file it is used here:

      static PyMethodDef UserFuncs[] = {
        // USER CONTENT HERE
	USERMODELFCT1D( c_line, 2 ),
	// END USER CONTENT HERE 
        { NULL, NULL, 0, NULL }
  
      };

    For each function to be added, USERMODELFCT1D is called, with the
    name of the function and the number of parameters for the
    function.  In this example, the name is c_line, and the number of
    parameters--that is, the length of the array p passed to
    c_line--is 2.

    The user need only add content after the "USER CONTENT HERE", and
    then only function prototypes for C code the user wrote in other
    file(s). 

3)  Install userfuncs module in the current working directory.

    Next, the user can create the user functions module, always called
    userfuncs, in the current working directory.

    There is a file setup.py that calls the C compiler and tells
    Python to create an interface to your C code.  The setup.py file
    needs to be told the name of your C or C++ file(s).  This can be
    done on the command line using the 'cfiles' argument; a single C
    file is named this way:

    % python setup.py cfiles=foo.c

    while multiple files are given in comma-delimited list:

    % python setup.py cfiles=foo.c,bar.cc

    (The suffix of the file name is used to determine if the file is C
    or C++ code.)

    The best way to use setup.py is to call it to build and install
    the code in one operation.  For this example, with the c_line.c
    file, you would call it like this:

    % python setup.py cfiles=c_line.c install --prefix=`pwd`

    If you wish to install the module elsewhere, set the prefix flag
    to a different directory.

    a) External Libraries

       It is also possible to link to external libraries.  That is,
       your code may refer to functions defined in other libraries.
       When creating the module, the setup.py file needs to be told
       the names of such libraries, and where to find them.

       For example, suppose your code refers to functions in your
       library libmy_funcs.a, which you've placed in the directory
       /home/me/lib.  And suppose that in turn, libmy_funcs.a refers
       to functions in the standard math library libm.a (usually
       found in a standard place such as /usr/lib).

       The setup.py file can accept lists of library names and
       directories, with the 'libs' and 'libdirs' arguments.  So to
       include these libraries in your build, you would call setup.py as

       % python setup.py cfiles=foo.c libs=my_funcs,m libdirs=/home/me/lib install --prefix=`pwd`

       This will tell the build process to include /home/me/lib as a
       directory that contains needed libraries; the build process
       will find libmy_funcs.a there.  (It will also find libm.a in
       /usr/lib, but you don't need to tell the build process to look
       there, since it already knows that /usr/lib is a place to look
       for needed libraries.)

       The 'libs' argument can contain a comma-delimited list of
       library names (you do need to remove the prefix "lib", and the
       suffixes ".a" or ".so" from the library names you list here).
       The 'libdirs' argument can contain a comma-delimited list of
       paths to search for needed libraries.

4)  Change PYTHONPATH to find the userfuncs module.

    The install created a new directory, lib, in the current working
    directory.  This is where the userfuncs module was installed.  To
    import the module, Python needs to be told where it is.

    If the user sourced the CIAO setup file, PYTHONPATH is already set
    to point to CIAO Python modules.  Add the current working
    directory to PYTHONPATH:

    % setenv PYTHONPATH ${PYTHONPATH}:${PWD}/lib/python2.5/site-packages

    If the user installed the userfuncs module in a different
    directory, that path should be added to PYTHONPATH.

5)  Add the user function to a user model.

    The demo script included, demo_c_model.py, demonstrates the use of
    a user model containing a C function.  The whole script can be run
    from the Unix prompt:

    % python
    >>> execfile("demo_c_model.py")

    The execfile function executes the code in demo_c_model.py; the
    Python session is still running so that the user may continue with
    interactive commands (e.g., change the value of one of the model
    parameters, and call plot_fit again to see the updated values
    generated with the user model).

    The most relevant lines in this file are:

      import userfuncs

      load_user_model(userfuncs.c_line, "c_line")
      add_user_pars("c_line", ["m","b"])

      set_model(1, c_line)
      
    The C user function module is imported.  The user model is
    created, with a reference to the C function.  Parameters ["m","b"]
    are added to the user model.  Finally, the user model is assigned
    to be the model fit to data set 1.

The user can add an arbitrary number of C and C++ functions to the
extension module the user creates, and access them all in a single
Python session.
