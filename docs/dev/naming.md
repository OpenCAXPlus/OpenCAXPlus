# Naming conventions

In general, we follow the [GNU C naming convention style](https://www.gnu.org/prep/standards/html_node/Names.html#Names).
In short use snake case when possible, such as for function name and variables. Reserve upper case for macros and enum constants.
File and folder names should use snake case too.

The above is only for C, in the case of C++, we haven't decide yet.

## constants

Use UPPER_SNAKE for constants.

## typedef

Since you are defining a type name, snake case is no longer suitable, so we use camelCase here.

There are 4 types of typedef in the framework's interface, type, struct, enum, func. The main reason we append suffix of Type, Struct, Enum, and Func is to avoid possible naming conflict.

In general the name should look like this:

> ocp[PascalCase Name][typedef type][Ptr]

Notice for function type, since it is always a function pointer, the Ptr is omitted in the end.

The regex matching pattern is as follows:

> (ocp)(([A-Z]+|[A-Z][a-z0-9]+)(\d|[A-Z][a-z0-9]+)\*)(Type|Enum|Struct|Func)(Ptr)?

Check if your name conform to the standard [here](https://regex101.com/r/RsEG2p/1).

Here are some examples:

| PascalCase Name | Typedef Type |   Ptr   | Full Name                    |
| :------------: | :----------: | :-----: | :--------------------------- |
|    String32    |     Type     |   No    | ocpString32Type              |
|   String128    |     Type     |   Yes   | ocpString128TypePtr          |
|      Data      |     Enum     |   No    | ocpDataEnum                  |
|     Status     |     Enum     |   No    | ocpStatusEnum                |
|   CSRSolver    |     Func     | Omitted | ocpCSRSolverFunc             |
|      Info      |     Func     | Omitted | ocpInfoFunc                  |
|      Vec       |    Struct    |   Yes   | ocpVecFloat64StructPtr       |
|   CSRSolver    |    Struct    |   Yes   | ocpCSRSolverContextStructPtr |
