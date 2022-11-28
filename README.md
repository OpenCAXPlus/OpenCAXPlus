![logo_title](https://user-images.githubusercontent.com/3356113/200965446-ec2a683a-620c-4f6a-83dc-ecb56cff38d1.png)

# OpenCAXPlusSDK

[![SDK Build & Test](https://github.com/OpenCAXPlus/OpenCAXPlusSDK/actions/workflows/sdk_build_test.yml/badge.svg)](https://github.com/OpenCAXPlus/OpenCAXPlusSDK/actions/workflows/sdk_build_test.yml)
[![Cli Build and test](https://github.com/OpenCAXPlus/OpenCAXPlusSDK/actions/workflows/cli_build_test.yml/badge.svg)](https://github.com/OpenCAXPlus/OpenCAXPlusSDK/actions/workflows/cli_build_test.yml)

The sdk documentation [site](https://sdk.opencax.plus).
Our [landing page](https://opencax.plus).

## For SDK developer

### Dev environment
Need to have golang and cmake installed.
```
sudo apt install golang-go cmake
```

### Usage

Initial run
```
git clone https://github.com/OpenCAXPlus/OpenCAXPlusSDK.git
cd OpenCAXPlusSDK
cmake --preset="linux-gnu-Debug" -S "."
cmake --build --preset="linux-gnu-Debug"
```

The initial run will build the OpenCAXPlus command line tool "ocp", you can use the ocp command to manage the SDK from now on. 
Following run
```
./ocp build
```

<!-- The logging library (spdlog) does not work with intel oneapi (classic) on mac, so you are expected to see different logging style for mac+intel vs other OS compiler combinations.

You shouldn't use any toolkit code from the framework.
Only use framework code from the toolkit if necessary, such as in the interface part of the toolkit. The core logic part of the toolkit should be as independent as possible. -->
