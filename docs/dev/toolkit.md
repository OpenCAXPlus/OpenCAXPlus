# Add new libraries to the toolkit

A toolkit library is the external library plus wrapper that follows the interface definition in our framework. 
The folder structure of a toolkit library should always look like this:
- README.md
- .gitignore
- CMakeLists.txt
- some source codes
- external
    - CMakeLists.txt
    - name-version.tar.gz 

## Rules you must follow

When we review your pull request for a new toolkit library, we will look into the following things.

### README.md

You must have a README.md file that explain basic things about your code.

### Modern cmake

You must use the modern cmake build system. Notice, it's not just cmake but you have to use modern cmake.

It is recommended to use the CMake macro provided by us, such as `ocp_toolkit`, to help you setup a standard library target. Check [here](cmake.md) for more information of available CMake macros.

The CMakeLists.txt file should create a target following the naming convention of "OCP::toolkit_{toolkit_folder_name}" that linked to the imported target from the external folder.
The target should also be linked to the **OCP::Interface**, and the wrapper functions should follow the interface defined in the framework.

### external folder

The external library must be places in the external folder, and 

```{admonition} No source code should be included
:class: warning

You should only include a compress source code following the convention of "name-version.tar.gz" in the external folder
```

The external folder should contain a CMakeLists.txt file that create an imported target to fulfill all requirements, such as linking libraries, include folders, compiler options, and etc., when using this external library.

It is recommended to use the CMake macro provided by us, such as `ocp_toolkit_external`, to help you setup a external library target. Check [here](cmake.md) for more information of available CMake macros.

## Things to notice for each toolkit

### Qt
You need to set the QT_EMAIL and QT_PASSWORD environment variables.

On MacOS, you need to manually click the dmg and install Qt. Here are the steps:
1. Select `Custom installation` and set the installation folder to `~/ocp/cache/Qt/5.12.10/install`. Then Next
2. Select `Archive` and `LTS` then select 5.12.10