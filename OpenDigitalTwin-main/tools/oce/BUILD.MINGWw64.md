This will help you build OCE with MinGW w64/MSYS using the Oce-Win-Bundle sources:

## 1. MinGW w64

- Download MinGW w64 from http://mingw-w64.sourceforge.net/
- Choose Mingw-builds Project, online Installer. (You can try others as well, but this one works...)
- When Installing use POSIX as Threading Model and SEH as Exception Model (others might work too, but are untested)
- Add the /bin directory of your MinGW install to your system's PATH-Variable (e.g. C:\mingw-builds\x64-4.8.1-posix-seh-rev5\mingw64\bin)
	
## 2. MSYS

- Download MSYS from Mingw-builds Sourceforge Repository: 
http://sourceforge.net/projects/mingwbuilds/files/external-binary-packages/
does not need installation, just extract into its own directory
	
## 3. CMake

- Install CMake >2.8

## 4. Open Cascade Community Edition

- Obtain a local copy of the current OCE repository 
        
        git clone git://github.com/tpaviot/oce.git
			
## 5. Oce-Win-Bundle
	
- To obtain the FreeImage, FreeType and gl2ps libraries clone the oce-win-bundle repository into its own directory:

        git clone https://github.com/QbProg/oce-win-bundle.git

## 6. Run CMake

- Point "Where is the source code:" to your local copy of OCE
- Build the binaries in a new specific build folder within OCE
- Hit Configure
- Choose "MSYS Makefiles" (NOT "MinGW Makefiles"!) with default native compilers
- When required: Point CMAKE_MAKE_PROGRAM to /bin/make.exe in your MSYS directory and configure again.
- Check OCE_USE_BUNDLE_SOURCE
- Check OCE_WITH_FREEIMAGE
- Check OCE_WITH_GL2PS
- Check OCE_VISUALISATION
- Use OPENMP as OCE_MULTITHREAD_LIBRARY
- Select your Installation directory with OCE_INSTALL_PREFIX
- Configure
- Select the directory of your oce-win-bundle clone with OCE_BUNDLE_ROOT_PATH
- Select your required build type (RELEASE,DEBUG..)
- Configure (and no entries should be red afterwards..)
- and Generate.
	
## 7. Run MSYS and make

- Start msys.bat in your MSYS-Directory (as an Administrator)
- cd into your build directory
- run 'make'
	
Compilation is now running. It can take several hours, depending on your machine.
	
## 8. Install

- run 'make install'
- If you want to compile as DEBUG, you might have to compile twice, once as RELEASE and once as DEBUG to obtain the required release and debug libraries.
- If you do not need Visualisation, GL2PS etc. just uncheck them.

Check for an updated Version of this File on the OCE Wiki on Github.
