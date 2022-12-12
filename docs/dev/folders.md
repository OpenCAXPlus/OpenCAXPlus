# Understand the folder structure

## Core folders:
- **Framework**: is a skeleton of application. It contains two parts. 
    - **interface** which are header files that define programming rules for the SDK, so that various parts can be connected easily. 
    - **lifecycle** which defines the procedures of how an application should be called. It setup the routine parts of an application for the users and expose certain slots to the user to customize. Lifecycle calls your function to build an executable. 
- **Toolkit**: is a collection of software libraries. These are handy functions that you can call to save your developing time. Libraries in the toolkit must conform (at least partially) to the *framework/interface*.
- **Starter**: is a collection of starter templates for SDK users to initiate various new projects. Each folder within the Starter is a standalone cmake project.
- **Cli**: is a golang cli application that handles the initiating, build, and packaging of the SDK project for both SDK developers and users.
- **cmake**: contains cmake macros to be used in CMakeLists.txt files.

## Other less important folders and files:
- .github: is the default github configuration folder containing things such as github actions
- .vscode: containing configuration files for the Visual Studio Code
- docs: contains the documentations using MyST markdown with sphinx
- .clang-format: clang-format configuration file
- CMakeLists.txt: is the cmake file
- CMakePresets.json: is the cmake presets file, you need to use at least cmake version 3.20 to use this file.
- LICENSE: our open source license