This is Gmsh, an automatic three-dimensional finite element mesh generator with
built-in pre- and post-processing facilities.

Gmsh is copyright (C) 1997-2017 C. Geuzaine and J.-F. Remacle, and is
distributed under the terms of the GNU General Public License, Version 2 or
later, with an exception to allow for easier linking with external
libraries. See LICENSE.txt and CREDITS.txt for more information.

See the doc/ and tutorial/ directories for documentation. The reference manual
is located in doc/texinfo/. See the demos/ directory and the web site
http://gmsh.info for additional examples.

Building Gmsh from its source code requires a C++ compiler and CMake
(http://cmake.org). Building the graphical user interface requires FLTK 1.3.2 or
higher (http://fltk.org), configured with OpenGL support. Support for
constructive solid geometry requires OpenCASCADE 6.9 or higher (version 7.1 is
highly recommended; http://www.opencascade.com).


Build Gmsh from the command line
--------------------------------

* Create a build directory, for example as a subdirectory of Gmsh's source
  directory:

    mkdir build

* Run cmake from within the build directory, pointing to Gmsh's source
  directory:

    cd build
    cmake ..

* To build and install Gmsh then simply type

    make
    make install

* To change build options you can use "ccmake" instead of "cmake", e.g.:

    ccmake ..

  or you can specify options directly on the command line. For example, you can
  use

    cmake -DCMAKE_PREFIX_PATH=/opt/local ..

  to specify the location of external packages installed in non-standard
  directories. You can use

    cmake -DCMAKE_INSTALL_PREFIX=/opt

  to change the installation directory. Or you can use

    cmake -DENABLE_FLTK=0 ..

  to build a version of Gmsh without the FLTK graphical interface. The list of
  all available configuration options is given in the reference manual.

* You can keep multiple builds with different build options at the same
  time. For example, you could configure a debug graphical build in a "bin"
  subdirectory with

    cd bin
    cmake -DCMAKE_BUILD_TYPE=Debug .. 
    make
    make install

  and configure a fairly minimal static library (with only the geometry and
  post-processing modules and the parser) in a "lib" subdirectory with

    cd lib
    cmake -DDEFAULT=0 -DENABLE_BUILD_LIB=1 -DENABLE_POST=1 -DENABLE_PARSER=1 ..
    make lib
    make install/fast

  (Note that "make install/fast" allows you to install only the target that you
  just built, i.e. "lib", and will not trigger the recompilation of the default
  target "gmsh".)

* To see a detailed compilation log use

    make VERBOSE=1


Build Gmsh using CMake's graphical user interface
-------------------------------------------------

* Launch CMake and fill-in the two top input fields (telling where the Gmsh
  source directory is located and where you want the Gmsh binary to be created).

* Click on "Add entry" and define the variable CMAKE_PREFIX_PATH, of type
  "PATH", pointing to the location(s) of any external package(s) (FLTK,
  BLAS/LAPACK, etc.) installed in non-standard directories.

* Click on "Configure" and choose your compiler.

* Optionally change some configuration options (re-run "Configure" every time
  you change some options).

* Once you are happy with all the configuration options, click on "Generate".

* Go to the build directory and build Gmsh using your chosen compiler.

