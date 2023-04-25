# Prepare for FengSim development on ubuntu 22.04 virtual machine

Total time 3h

## System requirment
Core:2
RAM:4Gb
DISK:64Gb
VMware ESXI 6.7 U2


## Prepare dependencies

```sh
sudo apt update # 1min
sudo apt upgrade # 5min
sudo apt install git cmake golang-go # 1min
sudo apt install gfortran ninja-build pkg-config mlocate libclang-dev libboost-all-dev # 2min
sudo apt install libfontconfig1-dev libxcb-glx0-dev libx11-xcb-dev libxcb-icccm4-dev libxcb-image0-dev libxcb-keysyms1-dev libxcb-randr0-dev libxcb-render-util0-dev libxcb-shape0-dev libxcb-sync0-dev libxcb-xfixes0-dev libxcb-xinerama0-dev libxcb-xkb-dev libxkbcommon-x11-dev libglu1-mesa-dev libgl1-mesa-dev libfreetype6-dev libglib2.0-dev mesa-common-dev libdbus-1-dev libicu-dev libinput-dev libxkbcommon-dev libsqlite3-dev libssl-dev libpng-dev libjpeg-dev libx11-dev libxcb1-dev libxext-dev libxi-dev libxcomposite-dev libxcursor-dev libxtst-dev libxrandr-dev libxcb-shm0-dev libxcb-util0-dev libxt-dev # 2min
go env -w GO111MODULE=on
go env -w GOPROXY=https://goproxy.cn,direct
# go env -w GOPROXY=https://mirrors.aliyun.com/goproxy/,direct
# go env -w GOPROXY=https://goproxy.io,direct
# go clean --modcache
```

After my testing Qt 5.12.10 has problem with gnu 11.3, so we still have to install gnu 10 for now.
```sh
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt install -y gcc-10 g++-10
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 100 --slave /usr/bin/gcov gcov /usr/bin/gcov-10 --slave /usr/bin/gcc-ar gcc-ar /usr/bin/gcc-ar-10 --slave /usr/bin/gcc-ranlib gcc-ranlib /usr/bin/gcc-ranlib-10
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 100
sudo update-alternatives --install /usr/bin/gfortran gfortran /usr/bin/gfortran-10 100
sudo update-alternatives --config g++
```

If using vmware, you can install the vm tools
```sh
sudo apt install open-vm-tools-desktop open-vm-tools # 1min
```

## Get source code

You need to setup your ssh key.

```sh
git clone git@github.com:OpenCAXPlus/OCP.git # 1min
cd OCP
```

## Initial build

```sh
cmake --preset="linux-gnu-Debug" -S "." # 1min
cmake --build --preset="linux-gnu-Debug" # 1min
ls ~/ocp/bin/ocp
export PATH=$PATH:~/ocp/bin
```

Now you can use the ocp command from command line.

## Install FengSim dependencies 
 
ocp build has the -t option to build a specific target.

```sh
export QT_EMAIL= ***
export QT_PASSWROD= ***
ocp build -t install_gmsh # 6min
ocp build -t install_OCE # 120min
```

## Build FengSim

ocp build has the -p option to build project located in path.

```sh
ocp build -p starter/fengsim
```

## Run FengSim

```sh
cd starter/fengsim
source LD.sh
./out/build/linux-gnu-Debug/FengSim
```
