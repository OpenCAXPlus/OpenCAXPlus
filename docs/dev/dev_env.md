# Development environment

## Recommended setup

### Code editor
vscode

### Compilers
gnu, intel oneapi

You need to use g++-10 for fenics, and g++-9 for M++
```
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 10
sudo update-alternatives --config g++
```

### Dependencies

Need to have golang and cmake installed.

::::{tab-set}

:::{tab-item} Linux
```sh
wget https://github.com/Kitware/CMake/releases/download/v3.26.0-rc3/cmake-3.26.0-rc3.tar.gz
tar xzf cmake-3.26.0-rc3.tar.gz
cd cmake-3.26.0-rc3
sudo apt install libssl-dev
./bootstrap
make
sudo make install

wget https://go.dev/dl/go1.20.1.linux-amd64.tar.gz
sudo rm -rf /usr/local/go 
sudo tar -C /usr/local -xzf go1.20.1.linux-amd64.tar.gz
export PATH=$PATH:/usr/local/go/bin

sudo apt install gfortran ninja-build pkg-config mlocate libclang-dev
sudo apt install libfontconfig1-dev libxcb-glx0-dev libx11-xcb-dev libxcb-icccm4-dev libxcb-image0-dev libxcb-keysyms1-dev libxcb-randr0-dev libxcb-render-util0-dev libxcb-shape0-dev libxcb-sync0-dev libxcb-xfixes0-dev libxcb-xinerama0-dev libxcb-xkb-dev libxkbcommon-x11-dev libglu1-mesa-dev libgl1-mesa-dev libfreetype6-dev libglib2.0-dev mesa-common-dev libdbus-1-dev libicu-dev libinput-dev libxkbcommon-dev libsqlite3-dev libssl-dev libpng-dev libjpeg-dev libx11-dev libxcb1-dev libxext-dev libxi-dev libxcomposite-dev libxcursor-dev libxtst-dev libxrandr-dev libxcb-shm0-dev libxcb-util0-dev libxt-dev 
/usr/bin/python3 -m pip install numpy
export PATH=$PATH:$HOME/.local/bin
```

:::

:::{tab-item} Mac
```sh
brew install go cmake
```
:::

:::{tab-item} Windows
Content 1
:::

::::
