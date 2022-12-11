![logo_title](docs/_static/ocp.png)

# OpenCAXPlus-SDK

[![SDK Build & Test](https://github.com/OpenCAXPlus/OpenCAXPlusSDK/actions/workflows/sdk_build_test.yml/badge.svg)](https://github.com/OpenCAXPlus/OpenCAXPlusSDK/actions/workflows/sdk_build_test.yml)
[![Cli Build and test](https://github.com/OpenCAXPlus/OpenCAXPlusSDK/actions/workflows/cli_build_test.yml/badge.svg)](https://github.com/OpenCAXPlus/OpenCAXPlusSDK/actions/workflows/cli_build_test.yml)

The main goal of this project to provide a SDK (software development kit) that does the tedious works in building an CAX application for you, such as
- create wrapper functions that allow the developer to use multiple open source libraries seamlessly
- installing and linking necessary libraries
- GUI interface
- cross platform project setup
- documentation/landing page setup
- automated CICD setup
- automated packaging and publishing

The SDK also aims to impose no rules on how the developers should write their application, developers still have full control of the core logic of your application.

Starting from our starter templates, developers are able to spawn an production ready application in no time.

Checkout the sdk [site](https://sdk.opencax.plus) for developer guide, user manuals and examples.
Also, checkout our organization [landing page](https://opencax.plus) for more of our projects.


![air foil](docs/_static/airfoil.jpg)
![cad](docs/_static/cad.jpg)

<!-- The logging library (spdlog) does not work with intel oneapi (classic) on mac, so you are expected to see different logging style for mac+intel vs other OS compiler combinations.

You shouldn't use any toolkit code from the framework.
Only use framework code from the toolkit if necessary, such as in the interface part of the toolkit. The core logic part of the toolkit should be as independent as possible. -->
