# Interface

Interface defines the rules for library developers.
By rules we mean a set of header files that defines the function interfaces, data structures to connect any external libraries to the SDK.

By following these rules,

- smoothen the learning curve for using a new libraries by giving the same type of library a uniform calling interface
- library developers no longer need to worry about how to connect with other libraries

Interface contains not variable declaration, assignment, etc. It is the role of the main program to initialize the variables or the functions.

## Interface by types

There are 4 types of interfaces

- backend: the part that calculation actually happens
- frontend: the user interface, either GUI or CLI
- communication: data passing between backend and frontend
- job: job management
