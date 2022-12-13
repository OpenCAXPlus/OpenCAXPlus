# Create a new starter template

```{admonition} Always try this before pull request!
:class: tip

Always make sure you can copy the template folder to any directory, then build and run the program.
```

## Rules you must follow

When we review your pull request for a new starter template, we will look into the following things.

### README.md

You must have a README.md file that explain basic things about your code.

### Modern cmake

You must use the modern cmake build system. Notice, it's not just cmake but you have to use modern cmake.

You must use CMakePresets.json file and follow our naming convention.

It is recommended to use the CMake macro provided by us, such as `ocp_embed_user_program`, to help you link your template with the SDK. Check [here](cmake.md) for more information of available CMake macros
