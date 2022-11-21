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

Use devtool.py to 
- build: `python3 devtool.py build -c gnu -b Debug -t all`
- test: `python3 devtool.py test -c gnu -b Debug `
- doc
- package: `python3 devtool.py test -c gnu -b Debug -t package`

### Basic usage
First, clone the repository
```sh
git clone https://github.com/OpenCAXPlus/OpenCAXPlusSDK.git
```
Then, use the devtool.py script to for basic usages, such as:
- build for all target
```sh
python3 devtool.py build -d Debug -c gnu -t all
```
- run test
```sh
python3 devtool.py test -d Release -c gnu
```
- build a starter, such as the FASPSolver
```sh
python3 devtool.py app -d Debug -c gnu -t all -p starter/FASPSolver
```
- run the built starter
```sh
python3 devtool.py app -d Debug -c gnu -t run -p starter/FASPSolver
```

## For SDK user
Use provided cli to
- initiate
- build 


## To use the SDK
1.  fork this repository
2.  (not fully ready yet) use our PPA at https://github.com/OpenCAXPlus/ppa