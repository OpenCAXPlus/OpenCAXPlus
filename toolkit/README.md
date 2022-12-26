# Using Toolkits
The OpenCAX+ Toolkit

Each plugin should have a similar structure. Add an external folder that contains the tar.gz download from external sources. Use the CMakeLists file to untar the file and add_subdirectory. Write a wrapper in the root path, create a target and remember to set the install for your target.
