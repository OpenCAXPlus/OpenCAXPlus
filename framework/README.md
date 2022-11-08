# OpenCAXPlusFramework

[![Linux GNU Build](https://github.com/OpenCAEPlus/OpenCAXPlusFramework/actions/workflows/linux_gnu_build.yml/badge.svg)](https://github.com/OpenCAEPlus/OpenCAXPlusFramework/actions/workflows/linux_gnu_build.yml)

The OpenCAXPlus Framework


- framework contains the core framework part
    - apps contains several example main programs using the OpenCAXPlus Framework
        - gui, example gui program
        - console, example console program
    - frontend, the gui part of the framework
    - backend, the calculation part of the framework
    - communication, the networking part of the framework
    - pbs, the portable batch system which performs job scheduling 
- plugins contains the official plugins for the OpenCAXPlus Framework
    - backend, plugins for the backend including IO, mesh, FEM, solver, etc.
    - frontend, plugins for the frontend including CAD, qt, vtk, etc.
- docs contains the documentation
- dev is to be used by external developers and define the necessary functions 
