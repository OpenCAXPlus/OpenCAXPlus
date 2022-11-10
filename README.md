![logo_title](https://user-images.githubusercontent.com/3356113/200965446-ec2a683a-620c-4f6a-83dc-ecb56cff38d1.png)

# OpenCAXPlusSDK

[![SDK Build & Test](https://github.com/OpenCAXPlus/OpenCAXPlusSDK/actions/workflows/sdk_build_test.yml/badge.svg)](https://github.com/OpenCAXPlus/OpenCAXPlusSDK/actions/workflows/sdk_build_test.yml)
[![Cli Build and test](https://github.com/OpenCAXPlus/OpenCAXPlusSDK/actions/workflows/cli_build_test.yml/badge.svg)](https://github.com/OpenCAXPlus/OpenCAXPlusSDK/actions/workflows/cli_build_test.yml)

The sdk documentation [site](https://sdk.opencax.plus).
Our [landing page](https://opencax.plus).
This is a superbuild for framework, cli, plugins, toolkits

For integerate all OpenCAXPlus projects and create an installer for user to use.
For creating SDK landing page and user manual.

## Dev environment

## For SDK developer

The logging library (spdlog) does not work with intel oneapi (classic) on mac, so you are expected to see different logging style for mac+intel vs other OS compiler combinations.

You shouldn't use any toolkit code from the framework.
Only use framework code from the toolkit if necessary, such as in the interface part of the toolkit. The core logic part of the toolkit should be as independent as possible.

## For SDK user
