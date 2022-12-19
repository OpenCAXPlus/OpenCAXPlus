# CMake files

## CPackConfig.cmake

This is the cpack configuration file, it will configure how we want to package the project for release.

## OCPEmbedUserProgram.cmake

Define a macro to be used in the starter template.

It will create a custom target:

- **run** to execute the compiled executable.

It accept the following variables:

| Variable name |    Variable type     | meaning                                                       |
| :-----------: | :------------------: | :------------------------------------------------------------ |
|   LIFECYCLE   |  one_value_keywords  | Choose the lifecycle main program you want to use             |
|     NAME      |  one_value_keywords  | Set your program's name                                       |
|    TARGET     |  one_value_keywords  | Set the name of your starter target (lifecycle depends on it) |
|     ROOT      |  one_value_keywords  | Root directory for the OCP SDK                                |
|     TOOLS     | multi_value_keywords | Choose the tools in toolkit you want to link to               |
|   COPYFILES   | multi_value_keywords | Files that you want to copy to the binary directory           |
|    USE_CXX    |       options        | Whether your starter project is using C++ or C                |

## OCPPrepareVariables.cmake

Convenient include file to set some common variables

## OCPToolkit.cmake

Define a macro to be used in the toolkit library.

It will create the following targets:

- **\${name}** a library for the toolkit library. Notice, the name is obtained automatically from your folder name.

It accept the following variables:

| Variable name |    Variable type     | meaning                                          |
| :-----------: | :------------------: | :----------------------------------------------- |
|     FILES     | multi_value_keywords | Source files in your library                     |
|     TOOLS     | multi_value_keywords | Other toolkit libraries that you need to link to |
|  NO_EXTERNAL  |       options        | Whether there is an external folder or not       |

## OCPToolkitExternal.cmake

Define a macro to be used in the external folder in a toolkit library. It will create several targets:

- **compress\_${software}** to compress a folder with the name ${software}-${version} into a 7z file.
- **external\_${software}** a INTERFACE IMPORTED library that work as a pseudo target so that you can link to the external library easily

It accept the following variables:

|    Variable name    |    Variable type     | meaning                                              |
| :-----------------: | :------------------: | :--------------------------------------------------- |
|      SOFTWARE       |  one_value_keywords  | The external library name, as in the 7z file         |
|       VERSION       |  one_value_keywords  | The version of the external library                  |
|      LIBRARIES      | multi_value_keywords | Libraries that you need to link if using the toolkit |
| INCLUDE_DIRECTORIES | multi_value_keywords | Include directories that you need to link to         |

## OCPSDKConfig.cmake

File cmake install