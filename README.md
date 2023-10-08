![logo_title](docs/_static/ocp.png)

# OpenCAXPlus OS

[![SDK Build & Test](https://github.com/OpenCAXPlus/OpenCAXPlusSDK/actions/workflows/sdk_build_test.yml/badge.svg)](https://github.com/OpenCAXPlus/OpenCAXPlusSDK/actions/workflows/sdk_build_test.yml)
[![Cli Build and test](https://github.com/OpenCAXPlus/OpenCAXPlusSDK/actions/workflows/cli_build_test.yml/badge.svg)](https://github.com/OpenCAXPlus/OpenCAXPlusSDK/actions/workflows/cli_build_test.yml)
[![Github pages](https://github.com/OpenCAXPlus/OCP-SDK/actions/workflows/gh_page.yml/badge.svg)](https://github.com/OpenCAXPlus/OCP-SDK/actions/workflows/gh_page.yml)

# Install
1. Navigate to the OpenDigitalTwin-main/cli directory in your terminal/command prompt.
2. Run the install script by entering: ./install.sh
3. Once installation is complete, launch QtCreator with: ./qtcreator.sh
4. In QtCreator, open the FENGSim.pro project file located at: starters/FENGSim/FENGSim.pro
5. Build the prepost module project in QtCreator.
	
# Showcases

![1](docs/_static/1.jpg)
![2](docs/_static/2.jpg)
![3](docs/_static/3.jpg)
![4](docs/_static/4.jpg)
![4](docs/_static/ros1.jpg)
![4](docs/_static/ros2.jpg)
![6](docs/_static/6.jpg)
![7](docs/_static/7.jpg)
![8](docs/_static/8.jpg)

<!-- The logging library (spdlog) does not work with intel oneapi (classic) on mac, so you are expected to see different logging style for mac+intel vs other OS compiler combinations.

You shouldn't use any toolkit code from the framework.
Only use framework code from the toolkit if necessary, such as in the interface part of the toolkit. The core logic part of the toolkit should be as independent as possible. -->
