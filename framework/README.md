# Framework

Framework is the core part of the SDK which defines the rules for both library developers and application developers. 

The rules come in two aspects:
1. header files in the [**interface**](/framework/interface/README) folder that defines the function interfaces, data structures to connect any external libraries to the SDK.
2. main functions in the [**lifecycle**](/framework/lifecycle/README) folder that defines various application execution procedures.

By following these rules,
- library developers no longer need to worry about how to connect with other libraries
- smoothen the learning curve for using a new libraries by giving the same type of library a uniform calling interface
- application developers can easily connect different libraries and switch between alternative libraries 