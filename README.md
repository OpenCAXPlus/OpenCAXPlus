![logo_title](docs/_static/ocp.png)

# OpenCAXPlus-SDK

[![SDK Build & Test](https://github.com/OpenCAXPlus/OpenCAXPlusSDK/actions/workflows/sdk_build_test.yml/badge.svg)](https://github.com/OpenCAXPlus/OpenCAXPlusSDK/actions/workflows/sdk_build_test.yml)
[![Cli Build and test](https://github.com/OpenCAXPlus/OpenCAXPlusSDK/actions/workflows/cli_build_test.yml/badge.svg)](https://github.com/OpenCAXPlus/OpenCAXPlusSDK/actions/workflows/cli_build_test.yml)
[![Github pages](https://github.com/OpenCAXPlus/OCP-SDK/actions/workflows/gh_page.yml/badge.svg)](https://github.com/OpenCAXPlus/OCP-SDK/actions/workflows/gh_page.yml)

The main goal of the OpenCAXPlus project is to provide an SDK (software development kit) that does the tedious work of building a CAX application. This includes but is not limited to

- creating wrappers that allow developers to use multiple open-source libraries seamlessly
- installing and linking necessary libraries
- providing GUI interface
- cross-platform project setup
- documentation/landing page setup
- automated CI/CD setup
- automated packaging and publishing

The SDK also aims to impose no rules on how developers should write their applications. Developers still have full control of the core logic of your application. Starting from our starter templates, developers can spawn a production-ready application in no time.

Check out the SDK [site](https://sdk.opencax.plus) for a developer guide, user manuals, and examples.
Also, check out our organization [landing page](https://opencax.plus) for more of our projects.

## User install

The install script, by default, will install the latest cli, aka ocp, for your current system. However you can also specify the version and distro name for the cli build explicitly.

```sh
# install a specific version of the cli
sh <(curl -s https://ocp-download.oss-cn-hongkong.aliyuncs.com/install.sh) 20230609 ubuntu-22.04
# Or use a shorter command default to latest and current system distro
sh <(curl -s https://ocp-download.oss-cn-hongkong.aliyuncs.com/install.sh)
```

The installation script will modify the PATH variable at the bottom of your ~/.bashrc file, you may revert this change if you don't want this behavior.

## Developer Build

```
cmake --preset="linux-gnu-Debug" -S "."
cmake --build --preset="linux-gnu-Debug"
```

## Developer publish

```sh
source scripts/linux/publish_cli.sh
rclone copy -P scripts/linux/install.sh ali:ocp-download
```


## Showcases

![1](docs/_static/1.jpg)
![2](docs/_static/2.jpg)
![3](docs/_static/3.jpg)
![4](docs/_static/4.jpg)
![6](docs/_static/6.jpg)
![7](docs/_static/7.jpg)
![8](docs/_static/8.jpg)


## TODO

1. include more external packages exodusii mstk jali wonton portage mesquite
2. parse ocp.yml and create ocp.lock file that list all used dependencies for current project.

