# Interface

The interface defines the rules for library developers. The rules are given as a set of header files that define the function interfaces and data structures to connect any external libraries to the SDK.

By following these rules,

- smoothen the learning curve for using a new library by giving the same type of library a uniform calling interface
- library developers no longer need to worry about how to connect with other libraries

The interface contains no variable declaration, assignment, etc. It is the role of the main program to initialize the variables or the functions.

## Interface by types

There are four types of interfaces

- backend: the part the calculation actually happens
- frontend: the user interface, either GUI or CLI
- communication: data passing between the backend and frontend
- job: job management
